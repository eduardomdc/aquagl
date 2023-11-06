#include "perlin.hpp"
#include "glm/common.hpp"
#include "glm/gtc/random.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"
#include <vector>
#include <algorithm>
#include <iostream>

Perlin::Perlin(int size){
    this->size = size;
    gradientVectors = std::vector<std::vector<std::vector<glm::vec3>>>(
        size+1, std::vector<std::vector<glm::vec3>>(
            size+1, std::vector<glm::vec3>(
                size+1, glm::vec3(0.f, 0.f, 0.f))));
    genRandomGrad();
}

void Perlin::genRandomGrad(){
    for (int i = 0; i<gradientVectors.size(); i++){
        for (int j = 0; j<gradientVectors[i].size(); j++){
            for (int k = 0; k<gradientVectors[i][j].size(); k++){
                gradientVectors[i][j][k] = glm::sphericalRand(1);
            }
        }
    }
}

float smoothstep (float a0, float a1, float x) {
	// Scale, and clamp x to 0..1 range
	// the cubic Hermite interpolation after clamping as implemented by GLSL
	float lo = 0;
	float hi = 1;
    x = std::clamp((x - a0)/(a1 - a0), lo, hi);
	return x * x * (3.0f - 2.0f * x);
}

float Perlin::sample(glm::vec3 pos){
    int cx, cy, cz;
    if (floor(pos.x) == pos.x && pos.x != 0.0) cx = pos.x-1;
    else cx = (int)floor(pos.x);
    if (floor(pos.y) == pos.y && pos.y != 0.0) cy = pos.y-1;
    else cy = (int)floor(pos.y);
    if (floor(pos.z) == pos.z && pos.z != 0.0) cz = pos.z-1;
    else cz = (int)floor(pos.z);
    // error checks
    if (cx < 0.0 || cx+1 > size){
        std::cout<<"cx is"<<cx<<std::endl;
        std::cout<<"posx is"<<pos.x<<std::endl;
        return 0.0;
    }
    if (cy < 0.0 || cy+1 > size){
        std::cout<<"cy is"<<cy<<std::endl;
        std::cout<<"posy is"<<pos.y<<std::endl;
        return 0.0;
    }
    if (cz < 0.0 || cz+1 > size){
        std::cout<<"cz is"<<cz<<std::endl;
        std::cout<<"posz is"<<pos.z<<std::endl;
        return 0.0;
    }

    std::vector<glm::vec3> gradients = {
        gradientVectors[cx][cy][cz],
        gradientVectors[cx+1][cy][cz],
        gradientVectors[cx][cy+1][cz],
        gradientVectors[cx][cy][cz+1],
        gradientVectors[cx+1][cy+1][cz],
        gradientVectors[cx][cy+1][cz+1],
        gradientVectors[cx+1][cy][cz+1],
        gradientVectors[cx+1][cy+1][cz+1],
    };
    std::vector<glm::vec3> offsets(8, glm::vec3(0,0,0));
    std::vector<float> dots(8, 0);

    for (int i=0; i<8; i++){
        offsets[i] = gradients[i] - pos;
        dots[i] = glm::dot(offsets[i], gradients[i]);
    }

    // first interpolate for plane z=0
    // interpolate y=0
    float edge00 = smoothstep(dots[0], dots[1], pos.x);
    // interpolate y=1
    float edge01 = smoothstep(dots[2], dots[4], pos.x);
    // interpolate between edges
    float plane0 = smoothstep(edge00, edge01, pos.y);
    // now interpolate for plane z=1
    edge00 = smoothstep(dots[3], dots[6], pos.x);
    edge01 = smoothstep(dots[5], dots[7], pos.x);
    float plane1 = smoothstep(edge00, edge01, pos.y);
    // interpolate between planes
    float value = smoothstep(plane0, plane1, pos.z);
    return 2*value-1;
}