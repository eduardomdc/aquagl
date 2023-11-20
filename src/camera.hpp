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
    //float height; // height from floor
    float yaw, pitch;

    void move(glm::vec3 newPos);
    //void update(float delta, Cave* cave);
    glm::mat4 view();
};

#endif
