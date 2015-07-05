cmake_minimum_required(VERSION 3.2)
project(modelloader)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_MODULE_PATH $ENV{HOME}/.brew/Cellar/sfml/2.3/share/SFML/cmake/Modules)
set(BREW_INCLUDE_DIR $ENV{HOME}/.brew/include)
set(SFML_ROOT $ENV{HOME}/.brew/lib)

find_package(SFML 2 REQUIRED system window graphics audio)

include_directories(includes)
include_directories(${BREW_INCLUDE_DIR})
include_directories($ENV{HOME}/libs/include)

set(SOIL_LIB $ENV{HOME}/libs/lib/libSOIL.a)

set(SOURCE_FILES
    srcs/main.cpp
    srcs/Camera.class.cpp
    srcs/Mesh.class.cpp
    srcs/Model.class.cpp
    srcs/Shader.class.cpp
    srcs/TextManager.class.cpp
    srcs/TimeManager.class.cpp
    srcs/Game.class.cpp
    srcs/Graphic.class.cpp
    srcs/InputManager.class.cpp
    srcs/Transform.class.cpp
    srcs/GameObject.class.cpp)

set(EXECUTABLE_NAME modelloader)

add_executable(${EXECUTABLE_NAME} ${SOURCE_FILES})
target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES} ${SOIL_LIB})
