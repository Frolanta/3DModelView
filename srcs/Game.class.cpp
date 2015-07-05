#include <Game.class.hpp>
#include <Graphic.class.hpp>
#include <TimeManager.class.hpp>
#include "general.hpp"
#include <Model.class.hpp>
#include <Camera.class.hpp>
#include <TextManager.class.hpp>
#include <TextManager.class.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <time.h>
#include <InputManager.class.hpp>
#include <GameObject.class.hpp>

Game::Game(void) {
}

Game::~Game(void) {
    Graphic::instance().terminate();

    return ;
}


void Game::startGame(void) {

    Graphic & g = Graphic::instance();


    TextManager & tm = TextManager::instance();
    tm.print("Loading Models, Textures and Shaders...", 245.0f, 340.0f, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
    g.display();

    Shader s;
    s.initialize("shaders/Shader.vertex", "shaders/Shader.fragment");
    Model model ("models/krabbs/krabbs.obj", &s);

    GameObject go;
    go.setModel(&model);


    //									 FOV		    Aspect Ratio			   Near / Far Planes
    this->_camera.setPerspective(glm::radians(45.0f), WIDTH / (float)HEIGHT, 0.01f, 100.0f);
    //					        Position	         Yaw	 Pitch
    this->_camera.positionCamera(0, 0.0f, 10.0f,		0.0f,		0.0f);

    InputManager::instance().setCamera(&_camera);


    while (g.isOpen()) {

        g.captureInput();
        TimeManager::instance().calculateFrameRate(false);

        g.clear();
        go.draw(&_camera);
        go.transform.rotation.y += TimeManager::instance().deltaTime;

        g.display();
    }

}
