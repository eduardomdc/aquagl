#ifndef BOIDS_HPP
#define BOIDS_HPP

#include <vector>
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "cave.hpp"

class BoidSystem;

class Boid {
public:
    Boid();
    glm::vec3 pos;
    glm::vec3 vel;
    glm::vec3 up;
    float delta;
    static BoidSystem* bs;
    static glm::mat4 scale;
    float phase; // wiggle phase
    void update(float delta);
};

class Killer { // predator boids
public:
    Killer();
    glm::vec3 pos;
    glm::vec3 vel;
    glm::vec3 up;
    float delta;
    static BoidSystem* bs;
    static glm::mat4 scale;
    float phase; // wiggle phase
    void update(float delta);
};

class BoidSystem{
public:
    BoidSystem(int amount, int killers, Cave* cave);
    Cave* cave;
    std::vector<Boid*> boids;
    std::vector<Killer*> killers;
    std::vector<float> verts;
    unsigned int VBO, VAO;
    void update(float delta);
    float neighboorRadius;
    float repellingForce;
    float cohesionForce;
    float alignmentForce;
};

#endif
