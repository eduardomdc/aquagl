#include "cave.hpp"
#include <cstdlib>
#include <vector>
#include "glm/geometric.hpp"
#include "light.hpp"
#include "marchingcubes.hpp"
#include "perlin.hpp"
#include "external/SimplexNoise.h"
#include <iostream>
#include <algorithm>
#include <GLFW/glfw3.h>

Cave::Cave(uint sizex, uint sizey, uint sizez){
    this->sizex = sizex;
    this->sizey = sizey;
    this->sizez = sizez;
    glfwInit();
    //perlin = new Perlin(30);
    simplex = new SimplexNoise(0.1);
    cavepoints = std::vector<std::vector<std::vector<float>>>(
        sizex, std::vector<std::vector<float>>(
            sizey, std::vector<float>(
                sizez, 0)));
    generatePoints();
    std::cout<<"Generating mesh..."<<std::endl;
    float time = (float)glfwGetTime();
    level = 0;
    vertices = marchingcubes(cavepoints, level);
    /*for(int i=0; i<vertices.size(); i+=5){
        std::cout<< "{";
        std::cout << vertices[i];
        std::cout <<", "<< vertices[i+1];
        std::cout <<", "<< vertices[i+2];
        std::cout <<"}{ "<< vertices[i+3];
        std::cout <<", "<< vertices[i+4]<<"}"<<std::endl;
    }*/
    time -= (float)glfwGetTime();
    std::cout<<"-- Done: "<<-time<<"s elapsed"<<std::endl;
    calculateNormals();
    light = new Light();
    ambientLight = new Light();
    light->pos = {sizex/2, 500.0, sizez/2};
    light->color = {1.0, 1.0, 1.0};
    light->dir = {0.0f, 1.0f, 0.0f};
    ambientLight->color = {0.1, 0.5, 1.0};
    ambientLight->intensity = 0.3;
    rockcolor = {0.95, 1.0, 1.0};
}

Cave::~Cave(){
    std::cout<<"Cave::Runnning destructor"<<std::endl;
    delete simplex;
    delete light;
    delete ambientLight;
}

float randomf(){
    // random float between -1 and 1
    float r = 2*(float(rand())/RAND_MAX) - 1;
    return r;
}

glm::vec3 Cave::fieldGrad(glm::vec3 pos){
    //returns gradient at pos vec(x,y,z)
    float epsilon = 1;
    float origin = field(pos.x, pos.y, pos.z); // value of field at vertex
    glm::vec3 normal;
    glm::vec3 samplePos = pos;
    float deltax = field(samplePos.x+epsilon, samplePos.y, samplePos.z);// value of field at +(epsilon,0,0)
    float deltay = field(samplePos.x, samplePos.y+epsilon, samplePos.z);
    float deltaz = field(samplePos.x, samplePos.y, samplePos.z+epsilon);
    normal = glm::vec3(
        (deltax-origin)/epsilon,
        (deltay-origin)/epsilon,
        (deltaz-origin)/epsilon
    );
    normal = glm::normalize(normal);
    return normal;
}

float distanceCenter(glm::vec3 pos, glm::vec3 center){
    return glm::length(pos-center);
}

void Cave::generatePoints(){
    std::cout<<"Generating terrain random values..."<<std::endl;
    float time = (float)glfwGetTime();
    //std::cout<<"Cave::generatePoints:: cavepoints.size:"<<cavepoints.size()<<std::endl;
    for (int i = 0; i<cavepoints.size(); i++){
        for (int j = 0; j<cavepoints[i].size(); j++){
            for (int k = 0; k<cavepoints[i][j].size(); k++){
                //cavepoints[i][j][k] = randomf();
                //std::cout<<cavepoints[i][j][k];
                //glm::vec3 pos = glm::vec3(i,j,k)*((float)perlin->size/cavepoints.size());
                //cavepoints[i][j][k] = perlin->sample(pos);
                cavepoints[i][j][k] = field(i, j, k);//(j-2);//glm::smoothstep(5.0f, 10.0f, (float)j)-(2-j);
                //if(i==0||k==0||i==cavepoints.size()-1||j==cavepoints.size()-1||k==cavepoints.size()-1) cavepoints[i][j][k] = 0;
            }
        }
    }
    time -= (float)glfwGetTime();
    std::cout<<"-- Done: "<<-time<<"s elapsed"<<std::endl;
}

float Cave::field(float x, float y, float z){
    return simplex->noise(x/30.0, y/30.0, z/30.0)
                                        +y*0.02
                                        -(1/(y*y+0.01))
                                        -(1/(y*y+1.))*simplex->noise(x/10., y/10., z/10.);
}

void Cave::calculateNormals(){
    std::cout<<"Calculating vertex normals..."<<std::endl;
    float time = (float)glfwGetTime();
    float epsilon = 1;
    for(int i=0; i<vertices.size(); i+= 5){
        glm::vec3 v(vertices[i], vertices[i+1], vertices[i+2]);
        float origin = field(v.x, v.y, v.z); // value of field at vertex
        glm::vec3 normal;
        glm::vec3 samplePos = v;
        float deltax = field(samplePos.x+epsilon, samplePos.y, samplePos.z);// value of field at +(epsilon,0,0)
        float deltay = field(samplePos.x, samplePos.y+epsilon, samplePos.z);
        float deltaz = field(samplePos.x, samplePos.y, samplePos.z+epsilon);
        normal = glm::vec3(
            (deltax-origin)/epsilon,
            (deltay-origin)/epsilon,
            (deltaz-origin)/epsilon
        );
        normal = glm::normalize(normal);
        // normal calculated, now push back gpu data structure:
        //  |vertex|normal|texcoord|
        //  |0    2|3    5|6   7|
        data.push_back(v.x);
        data.push_back(v.y);
        data.push_back(v.z);
        data.push_back(normal.x);
        data.push_back(normal.y);
        data.push_back(normal.z);
        data.push_back(vertices[i+3]);
        data.push_back(vertices[i+4]);
    }
    time -= (float)glfwGetTime();
    std::cout<<"-- Done: "<<-time<<"s elapsed"<<std::endl;
}
