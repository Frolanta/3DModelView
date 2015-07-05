#include <GameObject.class.hpp>

GameObject::GameObject(void) : _model(nullptr) {
    return ;
}

GameObject::~GameObject(void) {
    return ;
}

void GameObject::setModel(Model *model) {
    _model = model;
}

void GameObject::draw(Camera * camera) {
    if (_model)
        _model->draw(camera, transform.position, transform.scale, transform.rotation);
}
