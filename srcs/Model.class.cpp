#include <Model.class.hpp>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

#include "general.hpp"

Model::Model(std::string path, Shader * s){

    this->_shader = s;
    this->loadModel(path);
}

Model::~Model(void) {

}

void Model::draw(Camera * camera, glm::vec3 & position, glm::vec3 & scale, glm::vec3 & rotation) {

    this->_shader->turnOn();


    // Transformation matrices
    glm::mat4 projection = camera->getProjectionMatrix();
    glm::mat4 view = camera->getViewMatrix();


    glm::mat4 model;
    model = glm::translate(model, position);
    model = glm::scale(model, scale);
//
    model = glm::rotate(model, rotation.x, glm::vec3(1, 0, 0));	// x-axis
    model = glm::rotate(model, rotation.y, glm::vec3(0, 1, 0));	// y-axis
    model = glm::rotate(model, rotation.z, glm::vec3(0, 0, 1));	// z-axis

    GLint modelMatrixId = this->_shader->getVariable("model");
    GLint viewMatrixId = this->_shader->getVariable("view");
    GLint projectionMatrixId = this->_shader->getVariable("projection");

//    std::cout << modelMatrixId << std::endl;
//    std::cout << viewMatrixId << std::endl;
//    std::cout << projectionMatrixId << std::endl;

    this->_shader->setMatrix4(modelMatrixId, 1, GL_FALSE, glm::value_ptr(model));
    this->_shader->setMatrix4(viewMatrixId, 1, GL_FALSE, glm::value_ptr(view));
    this->_shader->setMatrix4(projectionMatrixId, 1, GL_FALSE, glm::value_ptr(projection));


    for(GLuint i = 0; i < this->_meshes.size(); i++)
        this->_meshes[i].draw();

    this->_shader->turnOff();
}

void Model::loadModel(std::string path) {

    // Read file via ASSIMP
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    // Check for errors
    if(!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }


    std::cout << "Number of total meshes: " << scene->mNumMeshes << std::endl;
    std::cout << "Animations: " << scene->mNumAnimations << std::endl;


    // Retrieve the directory path of the filepath
    this->_directory = path.substr(0, path.find_last_of('/'));

    // Process ASSIMP's root node recursively
    this->processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{

    // Process each mesh located at the current node
    for(GLuint i = 0; i < node->mNumMeshes; i++)
    {
        // The node object only contains indices to index the actual objects in the scene.
        // The scene contains all the data, node is just to keep stuff organized (like relations between nodes).
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        Mesh m = this->processMesh(mesh, scene);
        m.setShader(this->_shader);
        this->_meshes.push_back(m);
    }
    // After we've processed all of the meshes (if any) we then recursively process each of the children nodes
    for(GLuint i = 0; i < node->mNumChildren; i++)
    {
        this->processNode(node->mChildren[i], scene);
    }

}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    // Data to fill
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;

    // Walk through each of the mesh's vertices
    for(GLuint i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        glm::vec3 vector; // We declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
        // Positions
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.position = vector;
        // Normals
        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.normal = vector;
        // Texture Coordinates
        if(mesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
        {
            glm::vec2 vec;
            // A vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.texCoords = vec;
        }
        else
            vertex.texCoords = glm::vec2(0.0f, 0.0f);
        vertices.push_back(vertex);
    }

    // Now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for(GLuint i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // Retrieve all indices of the face and store them in the indices vector
        for(GLuint j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    // Process materials
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    // We assume a convention for sampler names in the shaders. Each diffuse texture should be named
    // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER.
    // Same applies to other texture as the following list summarizes:
    // Diffuse: texture_diffuseN
    // Specular: texture_specularN
    // Normal: texture_normalN

    // 1. Diffuse maps
    std::vector<Texture> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    // 2. Specular maps
    std::vector<Texture> specularMaps = this->loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());


    // Return a mesh object created from the extracted mesh data

    return Mesh(vertices, indices, textures);
}


std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
    std::vector<Texture> textures;
    for(GLuint i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        // Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        GLboolean skip = false;
        for(GLuint j = 0; j < _textures_loaded.size(); j++)
        {
            if(_textures_loaded[j].path == str)
            {
                textures.push_back(_textures_loaded[j]);
                skip = true; // A texture with the same filepath has already been loaded, continue to next one. (optimization)
                break;
            }
        }
        if(!skip)
        {   // If texture hasn't been loaded already, load it
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), this->_directory);
            texture.type = typeName;
            texture.path = str;
            textures.push_back(texture);
            this->_textures_loaded.push_back(texture);  // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
        }
    }
    return textures;
}


GLint Model::TextureFromFile(const char* path, std::string directory)
{
    //Generate texture ID and load texture data
    std::string filename(path);
    filename = directory + '/' + filename;
    GLuint textureID;
    glGenTextures(1, &textureID);
    int width,height;
    unsigned char * image = SOIL_load_image(filename.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
    // Assign texture to ID
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Parameters
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    SOIL_free_image_data(image);
    return textureID;
}
