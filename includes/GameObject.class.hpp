//
// Created by Remi BRIEUC on 05/07/15.
//

#ifndef GAMEOBJECT_CLASS_HPP
#define GAMEOBJECT_CLASS_HPP

#include <Transform.class.hpp>
#include <Model.class.hpp>
#include <Camera.class.hpp>

class GameObject {

public:
    GameObject(void);
    ~GameObject(void);

    void draw(Camera * camera);
    void setModel(Model * model);

    Transform transform;

private:
    Model * _model;



};

#endif //MODELLOADER_GAMEOBJECT_CLASS_HPP
