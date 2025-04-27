//
// Created by tommeh on 24.04.25.
//

#ifndef SYNAPSE_H
#define SYNAPSE_H

#include "node.h"

class Synapse {
public:
    Synapse(int start, int end, double weight, int innovationNumber): _start(start), _end(end), _weight(weight), historicalNumber(innovationNumber) {};
    ~Synapse() = default;
    void disable() {isEnabled = false;}
    void enable() {isEnabled = true;}
    void changeWeight(double newWeight) {_weight = newWeight;}

    bool isEnabled = true;
    int _start;
    int _end;
    double _weight;
    int historicalNumber;
};

#endif //SYNAPSE_H
