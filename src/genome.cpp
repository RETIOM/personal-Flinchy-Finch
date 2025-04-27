//
// Created by tommeh on 24.04.25.
//
#include "../include/genome.h"
#include <random>

Genome::Genome(int inputNumber, int outputNumber) {
    for (int i = 1; i <= inputNumber; i++) {
        inputNodes.emplace_back(NodeType::INPUT);
        nodes.insert({i, std::make_shared<Node>(inputNodes.back())});
    }
    for (int i = 1; i <= outputNumber; i++) {
        outputNodes.emplace_back(NodeType::OUTPUT);
        nodes.insert({i+inputNumber, std::make_shared<Node>(outputNodes.back())});
    }
    setIONodes(inputNumber, outputNumber);
}

Genome::Genome(Genome &fitterParent, Genome &otherParent) {
    // Handle IO nodes
    for (int i = 1; i <= INodes; i++) {
        inputNodes.emplace_back(NodeType::INPUT);
        nodes.insert({i, std::make_shared<Node>(inputNodes.back())});
    }
    for (int i = 1; i <= ONodes; i++) {
        outputNodes.emplace_back(NodeType::OUTPUT);
        nodes.insert({i+INodes, std::make_shared<Node>(outputNodes.back())});
    }

    // Handle Crossover
    const auto fitGenes = fitterParent.getConnectionGenes();
    const auto otherGenes = otherParent.getConnectionGenes();

    int i = 0, j = 0;

    while (i < fitGenes.size() && j < otherGenes.size()) {
        // Matching pair
        if (fitGenes[i].historicalNumber == otherGenes[j].historicalNumber) {
            if (static_cast<int>(getRandom(0,2)) == 1)
                connectionGenes.push_back(fitGenes[i]);
            else connectionGenes.push_back(otherGenes[j]);
            i++; j++;
        }
        // Disjoint in fitter parent
        else if (fitGenes[i].historicalNumber < otherGenes[j].historicalNumber) {
            connectionGenes.push_back(fitGenes[i]);
            i++;
        }
        // Disjoint in other parent
        else j++;
    }

    // Handles excess genes in fitter parent
    while (i < fitGenes.size()) {
        connectionGenes.push_back(fitGenes.at(i));
        i++;
    }

    // Mutation
    // mutate();

    // Prepares network for computation
    inferNetwork();
}



double Genome::getRandom(const float min, const float max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(min, max);

    return dis(gen);
}

std::vector<double> Genome::getOutput(const std::vector<double> &input) {
    setInputs(input);
    resetNetwork();
    std::vector<double> output;
    for (auto& nodeGene : outputNodes) {
        output.push_back(nodeGene.getOutput());
    }
    return output;
}

void Genome::setInputs(const std::vector<double> &input) {
    for (int i = 0; i < inputNodes.size(); i++) {
        inputNodes[i].setOutput(input[i]);
    }
}


void Genome::resetNetwork() {
    for (auto& [number, node] : nodes) {
        node->reset();
    }
}

void Genome::inferNetwork() {
    // Go through connections, check if start and end exist(add if not), setup previous and weights
    for (auto& synapse : connectionGenes) {
        if (!synapse.isEnabled) continue;

        if (!nodes.contains(synapse._start)) {
            nodes.insert({synapse._start, std::make_shared<Node>()});
        }
        if (!nodes.contains(synapse._end)) {
            nodes.insert({synapse._end, std::make_shared<Node>()});
        }

        nodes[synapse._end]->addPrevious(nodes[synapse._start], synapse._weight);
    }
}

bool Genome::compareHistorical(Synapse& synapse1, Synapse& synapse2) {
    return synapse1.historicalNumber < synapse2.historicalNumber;
}



