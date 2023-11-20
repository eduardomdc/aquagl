#ifndef CAVE_HPP
#define CAVE_HPP

#include <random>
#include <vector>
#include "external/SimplexNoise.h"
#include "perlin.hpp"
#include "light.hpp"
#include "glm/glm.hpp"


using namespace std;

class Cave {
public:
    Cave(uint sizex, uint sizey, uint sizez);
    ~Cave();
    float field(float x, float y, float z);
    void generatePoints();
    void generateVertices();
    void calculateNormals();
    unsigned int sizex, sizey, sizez;
    unsigned int trianglesCount;
    float level; // isosurface value
    Perlin* perlin;
    SimplexNoise* simplex;
    Light* light;
    Light* ambientLight;
    vector<vector<vector<float>>> cavepoints;
    std::vector<float> vertices;
    std::vector<float> data; //vertices with normals
    glm::vec3 rockcolor;
};
float randomf();
#endif