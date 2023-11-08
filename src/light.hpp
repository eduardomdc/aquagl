#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"

class Light {
public:
    Light();
    glm::vec3 pos;
    glm::vec3 dir;
    glm::vec3 color;
    float intensity;
};

#endif