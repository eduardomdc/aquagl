#ifndef APP_HPP
#define APP_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "camera.hpp"
#include "shader.hpp"
#include "cave.hpp"

class App {
public:
    App();
    Cave* cave;
    Camera camera;
    GLFWwindow* window;
    Shader* shader;
    float deltaTime = 0;
    float lastFrame = 0;
    float lastX=400, lastY = 300;

    glm::vec3 cubePositions[10] = {
        glm::vec3( 0.0f,  0.0f,  0.0f), 
        glm::vec3( 2.0f,  5.0f, -15.0f), 
        glm::vec3(-1.5f, -2.2f, -2.5f),  
        glm::vec3(-3.8f, -2.0f, -12.3f),  
        glm::vec3( 2.4f, -0.4f, -3.5f),  
        glm::vec3(-1.7f,  3.0f, -7.5f),  
        glm::vec3( 1.3f, -2.0f, -2.5f),  
        glm::vec3( 1.5f,  2.0f, -2.5f), 
        glm::vec3( 1.5f,  0.2f, -1.5f), 
        glm::vec3(-1.3f,  1.0f, -1.5f)  
    };
    unsigned int texture1;
    unsigned int texture2;
    unsigned int VBO, VAO, EBO;

    int init();
    void input();
    void render();
};

#endif