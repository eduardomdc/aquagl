#ifndef CAVE_HPP
#define CAVE_HPP

#include <random>
#include <vector>
#include "SimplexNoise.h"
#include "perlin.hpp"
#include "light.hpp"
#include "glm/glm.hpp"


using namespace std;

class Cave {
public:
    Cave(uint sizex, uint sizey, uint sizez);
    float field(float x, float y, float z);
    void generatePoints();
    void generateVertices();
    void calculateNormals();
    unsigned int sizex, sizey, sizez;
    unsigned int trianglesCount;
    Perlin* perlin;
    SimplexNoise* simplex;
    Light* light;
    Light* ambientLight;
    vector<vector<vector<float>>> cavepoints;
    std::vector<float> vertices;
    std::vector<float> data; //vertices with normals
    glm::vec3 rockcolor;
};

#endif