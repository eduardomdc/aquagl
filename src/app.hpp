#ifndef APP_HPP
#define APP_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "camera.hpp"
#include "shader.hpp"
#include "cave.hpp"
#include "boids.hpp"

class App {
public:
    App();
    Cave* cave;
    BoidSystem* boidsys;
    Camera camera;
    GLFWwindow* window;
    Shader* shader;
    Shader* fishader;
    float deltaTime = 0;
    float lastFrame = 0;
    float lastX=400, lastY = 300;
    unsigned int texture1;
    unsigned int fishtex;
    unsigned int VBO, VAO, EBO;

    int init();
    void input();
    void render();
    void update();
};

#endif
