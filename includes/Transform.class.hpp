//
// Created by Remi BRIEUC on 05/07/15.
//

#ifndef TRANSFORM_CLASS_HPP
#define TRANSFORM_CLASS_HPP

#include <glm/detail/type_vec.hpp>
#include <glm/detail/type_vec3.hpp>

class Transform {

public:
    Transform(void);
    ~Transform(void);

    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 rotation;


};

#endif //MODELLOADER_TRANSFORM_CLASS_HPP
