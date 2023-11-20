#include "camera.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"

Camera::Camera() {
    pos = glm::vec3(0,0,100);
    front = glm::vec3(0,0,-1);
    up = glm::vec3(0,1,0);
    yaw = -90.0f;
    pitch = 0.0f;
    //height = 3;
}

void Camera::move(glm::vec3 newPos){
    pos = newPos;
}

glm::mat4 Camera::view() {
    return glm::lookAt(pos, pos+front, up);
}

/*
void Camera::update(float delta, Cave* cave){
    // if above ground
    if (cave->field(pos.x, pos.y-height, pos.z) > cave->level){
        pos.y -= 0.1;
    } else {
        // how to find level point?
        return;
    }
}*/