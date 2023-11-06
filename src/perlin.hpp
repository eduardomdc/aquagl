#ifndef PERLIN_HPP
#define PERLIN_HPP

#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"

using namespace std;

class Perlin {
public:
    int size;
    Perlin(int size);
    void genRandomGrad();
    float sample(glm::vec3 pos);
    vector<vector<vector<glm::vec3>>> gradientVectors;
};

#endif