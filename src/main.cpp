#include <ctime>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"
#include "stb_image.h"
#include "shader.hpp"
#include "math.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "app.hpp"
#include <random>
#include <time.h>

App app = App();

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int main(){
    std::cout << "main:: starting" << std::endl;
    srand(time(nullptr));
    
    app.init();
    glfwSetCursorPosCallback(app.window, mouse_callback);
    glfwSetFramebufferSizeCallback(app.window, framebuffer_size_callback);

    //render loop
    while(!glfwWindowShouldClose(app.window)){
        // input
        app.input();
        // rendering commands
        app.render();
    }
    // deallocate resources
    glDeleteVertexArrays(1, &app.VAO);
    glDeleteBuffers(1, &app.VBO);
    glDeleteBuffers(1, &app.EBO);
    glDeleteProgram(app.shader->id);

    //close glfw
    glfwTerminate();

    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0,0,width,height);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), float(width) / float(height), 0.1f, 1000.0f);
	int projectionLoc = glGetUniformLocation(app.shader->id, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    app.shader->use();
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos){
	float xoffset = xpos - app.lastX;
	float yoffset = app.lastY - ypos;
    app.lastX = xpos;
    app.lastY = ypos;

    const float sensitivity = 0.05f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    app.camera.yaw += xoffset;
    app.camera.pitch += yoffset;

    if (app.camera.pitch > 89) app.camera.pitch = 89;
    if (app.camera.pitch < -89) app.camera.pitch = -89;

    glm::vec3 direction;
    direction.x = cos(glm::radians(app.camera.yaw )) * cos(glm::radians(app.camera.pitch));
    direction.y = sin(glm::radians(app.camera.pitch));
    direction.z = sin(glm::radians(app.camera.yaw)) * cos(glm::radians(app.camera.pitch));
    app.camera.front = glm::normalize(direction);
}