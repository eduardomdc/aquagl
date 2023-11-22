#include "boids.hpp"
#include "glm/ext/matrix_transform.hpp"
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
    if (glm::dot(dir, glm::normalize(boid->vel)) < -0.2f) return false;
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

    // side of box detection
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
    // terrain avoidance
    if (bs->cave->field(raycast.x, raycast.y, raycast.z) < bs->cave->level){
        glm::vec3 grad = bs->cave->fieldGrad(raycast);
        avoidance += 2.0f*grad;
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
    if (bs->cave->field(pos.x, pos.y, pos.z) < bs->cave->level){
        pos -= vel*delta;
        vel = glm::vec3(0.0f);
    }
}


glm::vec3 randomCavePoint(Cave* cave){
    glm::vec3 point;
    do {
        point.x = cave->sizex*(randomf()*0.5+0.5);
        point.y = cave->sizey/2.0;
        point.z = cave->sizez*(randomf()*0.5+0.5);
    } while (cave->field(point.x, point.y, point.z) < cave->level);
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
    verts =  {
2, 1.625, 1, 0.553452, 0.735419, 0.390961, 0, 0,
2, 2, 0.769231, 0.531098, 0.794179, 0.295321, 1, 0,
1.625, 2, 1, 0.465057, 0.806483, 0.365113, 0.5, 1,
2, 1.625, 1, 0.553452, 0.735419, 0.390961, 0, 0,
1.76923, 2, 2, 0.452244, 0.674538, 0.583501, 1, 0,
2, 1.76923, 2, 0.498196, 0.627943, 0.597904, 0.5, 1,
1.625, 2, 1, 0.465057, 0.806483, 0.365113, 0, 0,
1.76923, 2, 2, 0.452244, 0.674538, 0.583501, 0.5, 1,
2, 1.625, 1, 0.553452, 0.735419, 0.390961, 1, 0,
2, 1.76923, 2, 0.498196, 0.627943, 0.597904, 0, 0,
1.4, 2, 3, 0.346854, 0.559238, 0.752957, 1, 0,
2, 1.4, 3, 0.449462, 0.439744, 0.777566, 0.5, 1,
1.76923, 2, 2, 0.452244, 0.674538, 0.583501, 0, 0,
1.4, 2, 3, 0.346854, 0.559238, 0.752957, 0.5, 1,
2, 1.76923, 2, 0.498196, 0.627943, 0.597904, 1, 0,
2, 1.4, 3, 0.449462, 0.439744, 0.777566, 0, 0,
1.66667, 2, 4, 0.338714, 0.457164, 0.822359, 1, 0,
2, 1.66667, 4, 0.387752, 0.400327, 0.830293, 0.5, 1,
1.4, 2, 3, 0.346854, 0.559238, 0.752957, 0, 0,
1.66667, 2, 4, 0.338714, 0.457164, 0.822359, 0.5, 1,
2, 1.4, 3, 0.449462, 0.439744, 0.777566, 1, 0,
2, 1.66667, 4, 0.387752, 0.400327, 0.830293, 0, 0,
1.66667, 2, 4, 0.338714, 0.457164, 0.822359, 1, 0,
2, 2, 4.33333, 0.367425, 0.427693, 0.82588, 0.5, 1,
1.625, 2, 1, 0.465057, 0.806483, 0.365113, 0, 0,
2, 2, 0.769231, 0.531098, 0.794179, 0.295321, 1, 0,
2, 2.375, 1, 0.501467, 0.803654, 0.320423, 0.5, 1,
2, 2.23077, 2, 0.476418, 0.692015, 0.542348, 0, 0,
1.625, 2, 1, 0.465057, 0.806483, 0.365113, 1, 0,
2, 2.375, 1, 0.501467, 0.803654, 0.320423, 0.5, 1,
1.76923, 2, 2, 0.452244, 0.674538, 0.583501, 0, 0,
1.625, 2, 1, 0.465057, 0.806483, 0.365113, 0.5, 1,
2, 2.23077, 2, 0.476418, 0.692015, 0.542348, 1, 0,
2, 2.6, 3, 0.417642, 0.629508, 0.655205, 0, 0,
1.76923, 2, 2, 0.452244, 0.674538, 0.583501, 1, 0,
2, 2.23077, 2, 0.476418, 0.692015, 0.542348, 0.5, 1,
1.4, 2, 3, 0.346854, 0.559238, 0.752957, 0, 0,
1.76923, 2, 2, 0.452244, 0.674538, 0.583501, 0.5, 1,
2, 2.6, 3, 0.417642, 0.629508, 0.655205, 1, 0,
2, 2.33333, 4, 0.377769, 0.501562, 0.778284, 0, 0,
1.4, 2, 3, 0.346854, 0.559238, 0.752957, 1, 0,
2, 2.6, 3, 0.417642, 0.629508, 0.655205, 0.5, 1,
1.66667, 2, 4, 0.338714, 0.457164, 0.822359, 0, 0,
1.4, 2, 3, 0.346854, 0.559238, 0.752957, 0.5, 1,
2, 2.33333, 4, 0.377769, 0.501562, 0.778284, 1, 0,
2, 2, 4.33333, 0.367425, 0.427693, 0.82588, 0, 0,
1.66667, 2, 4, 0.338714, 0.457164, 0.822359, 1, 0,
2, 2.33333, 4, 0.377769, 0.501562, 0.778284, 0.5, 1,
2.375, 2, 1, 0.583181, 0.738605, 0.338176, 0, 0,
2, 2, 0.769231, 0.531098, 0.794179, 0.295321, 1, 0,
2, 1.625, 1, 0.553452, 0.735419, 0.390961, 0.5, 1,
2.375, 2, 1, 0.583181, 0.738605, 0.338176, 0, 0,
2, 1.76923, 2, 0.498196, 0.627943, 0.597904, 1, 0,
2.23077, 2, 2, 0.521021, 0.648616, 0.554829, 0.5, 1,
2, 1.625, 1, 0.553452, 0.735419, 0.390961, 0, 0,
2, 1.76923, 2, 0.498196, 0.627943, 0.597904, 0.5, 1,
2.375, 2, 1, 0.583181, 0.738605, 0.338176, 1, 0,
2.23077, 2, 2, 0.521021, 0.648616, 0.554829, 0, 0,
2, 1.4, 3, 0.449462, 0.439744, 0.777566, 1, 0,
2.6, 2, 3, 0.515137, 0.525437, 0.677162, 0.5, 1,
2, 1.76923, 2, 0.498196, 0.627943, 0.597904, 0, 0,
2, 1.4, 3, 0.449462, 0.439744, 0.777566, 0.5, 1,
2.23077, 2, 2, 0.521021, 0.648616, 0.554829, 1, 0,
2.6, 2, 3, 0.515137, 0.525437, 0.677162, 0, 0,
2, 1.66667, 4, 0.387752, 0.400327, 0.830293, 1, 0,
2.33333, 2, 4, 0.425662, 0.447509, 0.786478, 0.5, 1,
2, 1.4, 3, 0.449462, 0.439744, 0.777566, 0, 0,
2, 1.66667, 4, 0.387752, 0.400327, 0.830293, 0.5, 1,
2.6, 2, 3, 0.515137, 0.525437, 0.677162, 1, 0,
2, 2, 4.33333, 0.367425, 0.427693, 0.82588, 0, 0,
2.33333, 2, 4, 0.425662, 0.447509, 0.786478, 1, 0,
2, 1.66667, 4, 0.387752, 0.400327, 0.830293, 0.5, 1,
2.375, 2, 1, 0.583181, 0.738605, 0.338176, 0, 0,
2, 2.375, 1, 0.501467, 0.803654, 0.320423, 1, 0,
2, 2, 0.769231, 0.531098, 0.794179, 0.295321, 0.5, 1,
2.23077, 2, 2, 0.521021, 0.648616, 0.554829, 0, 0,
2, 2.375, 1, 0.501467, 0.803654, 0.320423, 1, 0,
2.375, 2, 1, 0.583181, 0.738605, 0.338176, 0.5, 1,
2, 2.23077, 2, 0.476418, 0.692015, 0.542348, 0, 0,
2, 2.375, 1, 0.501467, 0.803654, 0.320423, 0.5, 1,
2.23077, 2, 2, 0.521021, 0.648616, 0.554829, 1, 0,
2.6, 2, 3, 0.515137, 0.525437, 0.677162, 0, 0,
2, 2.23077, 2, 0.476418, 0.692015, 0.542348, 1, 0,
2.23077, 2, 2, 0.521021, 0.648616, 0.554829, 0.5, 1,
2, 2.6, 3, 0.417642, 0.629508, 0.655205, 0, 0,
2, 2.23077, 2, 0.476418, 0.692015, 0.542348, 0.5, 1,
2.6, 2, 3, 0.515137, 0.525437, 0.677162, 1, 0,
2.33333, 2, 4, 0.425662, 0.447509, 0.786478, 0, 0,
2, 2.6, 3, 0.417642, 0.629508, 0.655205, 1, 0,
2.6, 2, 3, 0.515137, 0.525437, 0.677162, 0.5, 1,
2, 2.33333, 4, 0.377769, 0.501562, 0.778284, 0, 0,
2, 2.6, 3, 0.417642, 0.629508, 0.655205, 0.5, 1,
2.33333, 2, 4, 0.425662, 0.447509, 0.786478, 1, 0,
2, 2, 4.33333, 0.367425, 0.427693, 0.82588, 0, 0,
2, 2.33333, 4, 0.377769, 0.501562, 0.778284, 1, 0,
2.33333, 2, 4, 0.425662, 0.447509, 0.786478, 0.5, 1, };   
    neighboorRadius = 5.0f;
    repellingForce = -0.1f;
    cohesionForce = 0.1f;
    alignmentForce = 0.05f;
    scale = glm::mat4(1.0f); // model size scaling
    scale = glm::scale(scale, glm::vec3(0.5f,0.5f,0.5f));

}

void BoidSystem::update(float delta){
    for (int i=0; i<boids.size(); i++){
        boids[i]->update(delta, this);
    }
}
