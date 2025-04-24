//
// Created by tommeh on 24.04.25.
//
#include "../include/genome.h"
#include <random>

Genome::Genome(int inputNumber, int outputNumber) {
    for (int i = 0; i < inputNumber; i++) {
        nodeGenes.emplace_back(NodeType::INPUT);
        inputNodes.push_back(&nodeGenes.back());
    }
    for (int i = 0; i < outputNumber; i++) {
        nodeGenes.emplace_back(NodeType::OUTPUT, getRandom());
        outputNodes.push_back(&nodeGenes.back());
    }
}

double Genome::getRandom() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-1.0, 1.0);

    return dis(gen);
}

std::vector<double> Genome::getOutput(std::vector<double> input) {
    setInputs(input);
    resetNetwork();
    std::vector<double> output;
    for (auto nodeGene : outputNodes) {
        output.push_back(nodeGene->getOutput());
    }
    return output;
}

void Genome::setInputs(std::vector<double> &input) {
    for (int i = 0; i < inputNodes.size(); i++) {
        inputNodes[i]->setOutput(input[i]);
    }
}


void Genome::resetNetwork() {
    for (auto& node : nodeGenes) {
        node.reset();
    }
}

void Genome::prepareNetwork() {
    for (auto& synapse : connectionGenes) {
        synapse._end.addPrevious(synapse._start, synapse._weight);
    }
}


