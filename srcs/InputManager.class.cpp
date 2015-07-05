#include <InputManager.class.hpp>
#include <TimeManager.class.hpp>

InputManager::InputManager(void) {
    return  ;
}

InputManager::~InputManager(void) {
    return ;
}

InputManager& InputManager::instance(void) {
    static InputManager inst;
    return inst;
}

void InputManager::setCamera(Camera *cam) {
    _camera = cam;
}

void InputManager::processInput(eKey key) {


    switch (key) {
        case KEYUP :
            _camera->moveCamera(_camera->getSpeed() * TimeManager::instance().deltaTime);
            break ;
        case KEYDOWN :
            _camera->moveCamera(-1 * _camera->getSpeed() * TimeManager::instance().deltaTime);
            break ;
        case KEYLEFT :
            _camera->strafe(-1 * _camera->getSpeed() * TimeManager::instance().deltaTime);
            break ;
        case KEYRIGHT :
            _camera->strafe(_camera->getSpeed() * TimeManager::instance().deltaTime);
            break ;
        default:
            break ;
    }
}

void InputManager::moveMoved(glm::vec2 move) {
    _camera->setViewByMouse(move.x, move.y);
}
