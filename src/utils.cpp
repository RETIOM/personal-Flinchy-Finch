//
// Created by tommeh on 29.04.25.
//
#include "../include/utils.h"
#include <random>


double utils::getRandom(float min, float max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(min, max);

    return dis(gen);
}
