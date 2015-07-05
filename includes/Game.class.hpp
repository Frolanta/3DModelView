#ifndef _GAME_CLASS_HPP_
#define _GAME_CLASS_HPP_

#include <Camera.class.hpp>
#include <list>

class Game {

public:
    Game(void);
    ~Game(void);

    void startGame(void);


private:
    Camera      _camera;


};

#endif //42RUN_GAME_CLASS_HPP
