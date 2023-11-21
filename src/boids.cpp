#include "boids.hpp"
#include "glm/ext/quaternion_geometric.hpp"
#include "glm/geometric.hpp"
#include "glm/gtc/random.hpp"
#include <iostream>
#include <vector>

Boid::Boid(){
    up = glm::vec3(0.0f, 1.0f, 0.0f);
}

bool inVisionCone(Boid* boid, Boid* neigh){
    // returns true if neigh is in vision cone of boid
    // we can check the dot product between normalized boid->neigh and boid velocity
    // the closer to -1 the more neigh is directly behind boid.
    glm::vec3 dir = glm::normalize(neigh->pos-boid->pos);
    if (glm::dot(dir, glm::normalize(boid->vel)) < 0.0f) return false;
    else return true;
}

void Boid::update(float delta, BoidSystem* bs){
    
    unsigned int count = 0; // number of neighbors
    glm::vec3 repelling = glm::vec3(0.0f, 0.0f, 0.0f); // repelling component of velocity
    glm::vec3 cohesion = glm::vec3(0.0f, 0.0f, 0.0f); // cohesion component of velocity
    glm::vec3 alignment = glm::vec3(0.0f, 0.0f, 0.0f); // alignment component
    glm::vec3 averagePos = glm::vec3(0.0f, 0.0f, 0.0f); // average position of neighbors
    glm::vec3 averageVel = glm::vec3(0.0f, 0.0f, 0.0f); // average velocity of neighbors
    
    for (int i=0; i<bs->boids.size(); i++){
        if (bs->boids[i] == this) continue; // no self interaction!
        float distance = glm::distance(bs->boids[i]->pos, pos);

        // find close boids
        if (distance < bs->neighboorRadius && inVisionCone(this, bs->boids[i])){
            count++;
            //repelling component
            glm::vec3 toBoid = bs->boids[i]->pos - pos;
            repelling += (1/distance)*bs->repellingForce*glm::normalize(toBoid);
            // averaging position
            averagePos += bs->boids[i]->pos;
            // averaging velocity
            averageVel += bs->boids[i]->vel;
        } else continue;
    }

    // collision avoidance
    // boundaries
    glm::vec3 avoidance = glm::vec3(0.0f, 0.0f , 0.0f); // avoidance component
    float vision = 5.0f;
    glm::vec3 raycast = pos + glm::normalize(vel)*vision;
    if (pos.x < 5.0f){
        avoidance += 0.5f*glm::vec3(1.0f, 0.0f, 0.0f);
    }
    if (pos.x > bs->cave->sizex -5.0f){
        avoidance -= 0.5f*glm::vec3(1.0f, 0.0f, 0.0f);
    }
    if (pos.y < 5.0f){
        avoidance += .5f*glm::vec3(0.0f, 1.0f, 0.0f);
    }
    if (pos.y > bs->cave->sizey-5.0f){
        avoidance -= .5f*glm::vec3(0.0f, 1.0f, 0.0f);
    }
    if (pos.z < 5.0f){
        avoidance += .5f*glm::vec3(0.0f, 0.0f, 1.0f);
    }
    if (pos.z > bs->cave->sizez-5.0f){
        avoidance -= .5f*glm::vec3(0.0f, 0.0f, 1.0f);
    }

    if (count > 0){
        averagePos = averagePos/(float)count;
        cohesion = bs->cohesionForce*glm::normalize(averagePos-pos);
        averageVel = averageVel/(float)count;
        alignment = bs->alignmentForce*glm::normalize(averageVel-vel);
    }
    vel += repelling + cohesion + alignment + avoidance;
    vel = 5.0f*glm::normalize(vel);
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
    std::cout<<"BoidSystem::Contructor()"<<std::endl;
    this->cave = cave;
    for (int i=0; i<amount; i++){
        Boid* boid = new Boid();
        boid->pos = randomCavePoint(cave);
        boid->vel = glm::sphericalRand(1.0f);
        boids.push_back(boid);
    }
    verts = {
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f
    };
    neighboorRadius = 5.0f;
    repellingForce = -0.1f;
    cohesionForce = 0.1f;
    alignmentForce = 0.05f;
}

void BoidSystem::update(float delta){
    for (int i=0; i<boids.size(); i++){
        boids[i]->update(delta, this);
    }
}
