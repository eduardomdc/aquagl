#include "boids.hpp"
#include "glm/gtc/random.hpp"
#include <iostream>

Boid::Boid(){
    up = glm::vec3(0.0f, 1.0f, 0.0f);
}

void Boid::update(float delta){
    pos += vel*delta;
}


glm::vec3 randomCavePoint(Cave* cave){
    glm::vec3 point;
    point.x = cave->sizex*(randomf()*0.5+0.5);
    point.y = cave->sizey/2.0;
    point.z = cave->sizez*(randomf()*0.5+0.5);
    return point;
}

BoidSystem::BoidSystem(int amount, Cave* cave){
    cave = cave;
    for (int i=0; i<amount; i++){
        Boid* boid = new Boid();
        boid->pos = randomCavePoint(cave);
        boid->vel = glm::sphericalRand(1.0f);
        boids.push_back(boid);
    }
    verts = {
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f
        };
}

void BoidSystem::update(float delta){
    for (int i=0; i<boids.size(); i++){
        boids[i]->update(delta);
    }
}