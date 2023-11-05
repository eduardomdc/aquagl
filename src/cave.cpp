#include "cave.hpp"
#include <cstdlib>
#include <vector>
#include "marchingcubes.hpp"
#include <iostream>

Cave::Cave(uint sizex, uint sizey, uint sizez){
    this->sizex = sizex;
    this->sizey = sizey;
    this->sizez = sizez;
    cavepoints = std::vector<std::vector<std::vector<float>>>(
        sizex, std::vector<std::vector<float>>(
            sizey, std::vector<float>(
                sizez, 0)));
    generatePoints();
    vertices = marchingcubes(cavepoints, 0.6);
    /*for(int i=0; i<vertices.size(); i+=5){
        std::cout<< "{";
        std::cout << vertices[i];
        std::cout <<", "<< vertices[i+1];
        std::cout <<", "<< vertices[i+2];
        std::cout <<"}{ "<< vertices[i+3];
        std::cout <<", "<< vertices[i+4]<<"}"<<std::endl;
    }*/
}

float randomf(){
    float r = 2*(float(rand())/RAND_MAX) - 1;
    return r;
}

void Cave::generatePoints(){
    //std::cout<<"Cave::generatePoints:: cavepoints:"<<std::endl;
    //std::cout<<"Cave::generatePoints:: cavepoints.size:"<<cavepoints.size()<<std::endl;
    for (int i = 0; i<cavepoints.size(); i++){
        for (int j = 0; j<cavepoints[i].size(); j++){
            for (int k = 0; k<cavepoints[i][j].size(); k++){
                cavepoints[i][j][k] = randomf();
                //std::cout<<cavepoints[i][j][k];
            }
        }
    }
    //std::cout<<std::endl;
}