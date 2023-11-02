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
#include "cavegen.hpp"

void processInput(GLFWwindow* window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main(){
    Cavegen cavegen = Cavegen();
    
    std::cout << "main:: starting" << std::endl;
    
    cavegen.init();
    //render loop
    while(!glfwWindowShouldClose(cavegen.window)){
        // input
        processInput(cavegen.window);
        // rendering commands
        cavegen.render();
    }
    // deallocate resources
    glDeleteVertexArrays(1, &cavegen.VAO);
    glDeleteBuffers(1, &cavegen.VBO);
    glDeleteBuffers(1, &cavegen.EBO);
    glDeleteProgram(cavegen.shader->id);

    //close glfw
    glfwTerminate();

    return 0;
}
