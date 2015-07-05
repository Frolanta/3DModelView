#ifndef _GRAPHIC_CLASS_HPP_
#define _GRAPHIC_CLASS_HPP_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <general.hpp>

#include <Camera.class.hpp>

class Graphic {

public:
    static Graphic & instance(void);
    ~Graphic(void);

    void terminate(void);


    void clear(void);
    void display(void);

    void captureInput(void);

    bool isOpen(void);


private:
    Graphic(void);

    GLFWwindow* _window;
    bool        _open;

};


#endif //42RUN_GRAPHIC_CLASS_HPP
