#include "camera.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"

Camera::Camera() {
    pos = glm::vec3(0,0,0);
    front = glm::vec3(0,0,-1);
    up = glm::vec3(0,1,0);
    yaw = -90.0f;
    pitch = 0.0f;
}

void Camera::move(glm::vec3 newPos){
    pos = newPos;
}

glm::mat4 Camera::view() {
    return glm::lookAt(pos, pos+front, up);
}
