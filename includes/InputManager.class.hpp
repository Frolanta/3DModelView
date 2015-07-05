//
// Created by Remi BRIEUC on 05/07/15.
//

#ifndef INPUTMANAGER_CLASS_HPP
#define INPUTMANAGER_CLASS_HPP

#include <Camera.class.hpp>
#include <general.hpp>

class InputManager {

public:
    static InputManager &  instance(void);
    ~InputManager(void);

    void processInput(eKey key);
    void moveMoved(glm::vec2 move);

    void setCamera(Camera * cam);

private:

    InputManager(void);
    Camera * _camera;

};

#endif //MODELLOADER_INPUTMANAGER_CLASS_HPP
