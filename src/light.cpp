#include "light.hpp"

Light::Light(){
    color = {1.0, 0.0, 0.0};
    pos = {100, 100, 0};
    dir = {0, 1, 0};
    intensity = 1;
}