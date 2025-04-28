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
        // Matching pair(handle mutateEnabled)
        if (fitGenes[i].historicalNumber == otherGenes[j].historicalNumber) {
            // Choose random version
            if (static_cast<int>(getRandom(0,2)) == 1)
                connectionGenes.push_back(fitGenes[i]);
            else connectionGenes.push_back(otherGenes[j]);

            // Handle mutation
            if (fitGenes[i].isEnabled && fitGenes[j].isEnabled) {
                const double probMutDis = 0.75;
                if (getRandom(0,1) < probMutDis) {connectionGenes.back().changeEnable();}
            }
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
    resetNetwork();
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

double Genome::compareSimilarity(Genome &genome) {
    const auto selfGenome = getConnectionGenes();
    const auto otherGenome = getConnectionGenes();

    const auto N = std::max(selfGenome.size(), otherGenome.size());
    const auto n = std::min(selfGenome.size(), otherGenome.size());

    int excess = 0, disjoint = 0, equal = 0;
    double weightDifference = 0.0;

    int i = 0, j = 0;

    while (i < selfGenome.size() && j < otherGenome.size()) {
        if (selfGenome[i].historicalNumber == otherGenome[j].historicalNumber) {
            weightDifference += std::abs(selfGenome[i]._weight - otherGenome[j]._weight);
            i++; j++; equal++;
        }
        else if (selfGenome[i].historicalNumber < otherGenome[j].historicalNumber) i++;
        else j++;
        disjoint++;
    }

    excess = N-n-disjoint;

    return c1*excess/N+c1*disjoint/N+c3*weightDifference/equal;
}



void Genome::mutateWeight() {
    const double perturbeOverRandom = 0.9;

    if (getRandom(0, 1) > perturbeOverRandom) {
        randomWeight();
    }
    else perturbeWeight();
}

void Genome::randomWeight() {
    const auto id = static_cast<int>(getRandom(0,connectionGenes.size()));
    connectionGenes[id].changeWeight(getRandom());
}

void Genome::perturbeWeight() {
    const auto id = static_cast<int>(getRandom(0,connectionGenes.size()));
    connectionGenes[id].perturbeWeight(getRandom(0,2));
}

void Genome::mutateNode() {
    const auto synapseId = static_cast<int>(getRandom(0,connectionGenes.size()));
    const auto start = connectionGenes[synapseId]._start;
    const auto end = connectionGenes[synapseId]._end;

    int newNodeNumber = getNodeNumber()+getIONodes();
    nodes.insert({newNodeNumber, std::make_shared<Node>()});

    addConnection(start, newNodeNumber, 1);
    addConnection(newNodeNumber, end, connectionGenes[synapseId]._weight);
    connectionGenes[synapseId].changeEnable();
}


void Genome::addConnection(int start, int end, double weight) {
    auto historicalNumber = getInnovation();
    connectionGenes.emplace_back(start, end, weight, historicalNumber);
}

// This wont work :); No assurance that the end is AFTER the start => topsort needs to be implemented :(
void Genome::mutateConnection() {
    int startNodeId, endNodeId;
    do {
        startNodeId = static_cast<int>(getRandom(0,nodes.size()));
    } while (startNodeId >= INodes && startNodeId < getIONodes());
    endNodeId = static_cast<int>(getRandom(INodes,nodes.size()));
}

void Genome::mutate() {
    constexpr double probMutWeight = 0.8;
    constexpr double probMutConn = 0.05;
    constexpr double probMutNode = 0.03;

    if (getRandom(0,1) < probMutWeight) mutateWeight();
    if (getRandom(0,1) < probMutConn) mutateConnection();
    if (getRandom(0,1) < probMutNode) mutateNode();
}












