#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "cave.hpp"

class Camera {
public:
    Camera();
    glm::vec3 pos;
    glm::vec3 front;
    glm::vec3 up;
    float yaw, pitch;
    bool fps;

    void move(glm::vec3 newPos);
    glm::mat4 view();
};

#endif
