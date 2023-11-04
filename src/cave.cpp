#include "cave.hpp"
#include <cstdlib>
#include <vector>

Cave::Cave(uint sizex, uint sizey, uint sizez){
    this->sizex = sizex;
    this->sizey = sizey;
    this->sizez = sizez;
    std::vector<std::vector<std::vector<int>>> cavepoints(
        sizex, std::vector<std::vector<int>>(
            sizey, std::vector<int>(
                sizez, 0)));
}

float randomf(){
    float r = 2*(float(rand())/RAND_MAX) - 1;
    return r;
}

void Cave::generatePoints(){
    for (int i = 0; i<sizex; i++){
        for (int j = 0; j<sizey; j++){
            for (int k = 0; k<sizez; k++){
                cavepoints[i][j][k] = randomf();
            }
        }
    }
}