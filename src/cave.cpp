#include "cave.hpp"
#include <cstdlib>
#include <vector>
#include "glm/geometric.hpp"
#include "marchingcubes.hpp"
#include "perlin.hpp"
#include "SimplexNoise.h"
#include <iostream>

Cave::Cave(uint sizex, uint sizey, uint sizez){
    this->sizex = sizex;
    this->sizey = sizey;
    this->sizez = sizez;
    perlin = new Perlin(30);
    simplex = new SimplexNoise(0.1);
    cavepoints = std::vector<std::vector<std::vector<float>>>(
        sizex, std::vector<std::vector<float>>(
            sizey, std::vector<float>(
                sizez, 0)));
    generatePoints();
    std::cout<<"Generating mesh..."<<std::endl;
    vertices = marchingcubes(cavepoints, 0.0);
    /*for(int i=0; i<vertices.size(); i+=5){
        std::cout<< "{";
        std::cout << vertices[i];
        std::cout <<", "<< vertices[i+1];
        std::cout <<", "<< vertices[i+2];
        std::cout <<"}{ "<< vertices[i+3];
        std::cout <<", "<< vertices[i+4]<<"}"<<std::endl;
    }*/
    std::cout<<"-- Done"<<std::endl;
}

float randomf(){
    float r = 2*(float(rand())/RAND_MAX) - 1;
    return r;
}

float distanceCenter(glm::vec3 pos, glm::vec3 center){
    return glm::length(pos-center);
}

void Cave::generatePoints(){
    std::cout<<"Generating random values..."<<std::endl;
    //std::cout<<"Cave::generatePoints:: cavepoints.size:"<<cavepoints.size()<<std::endl;
    for (int i = 0; i<cavepoints.size(); i++){
        for (int j = 0; j<cavepoints[i].size(); j++){
            for (int k = 0; k<cavepoints[i][j].size(); k++){
                //cavepoints[i][j][k] = randomf();
                //std::cout<<cavepoints[i][j][k];
                //glm::vec3 pos = glm::vec3(i,j,k)*((float)perlin->size/cavepoints.size());
                //cavepoints[i][j][k] = perlin->sample(pos);
                cavepoints[i][j][k] = simplex->noise(i/30.0, j/30.0, k/30.0);
                glm::vec3 center = glm::vec3(cavepoints.size()/2, cavepoints.size()/2, cavepoints.size()/2);
                //cavepoints[i][j][k] -= distanceCenter(glm::vec3(i,j,k), center)*0.02;
            }
        }
    }
    std::cout<<"-- Done"<<std::endl;
}