//
// Created by tommeh on 24.04.25.
//
#include "../include/genome.h"
#include <random>

#include <iostream>

Genome::Genome(int inputNumber, int outputNumber) {
    for (int i = 1; i <= inputNumber; i++) {
        auto newInputNode = std::make_shared<Node>(NodeType::INPUT, i);
        inputNodes.push_back(newInputNode);
        nodes[i] = newInputNode;
    }
    for (int i = 1; i <= outputNumber; i++) {
        auto newOutputNode = std::make_shared<Node>(NodeType::OUTPUT, i+inputNumber);
        outputNodes.push_back(newOutputNode);
        nodes[i+inputNumber] = newOutputNode;
    }
    setIONodes(inputNumber, outputNumber);
}

Genome::Genome(Genome &fitterParent, Genome &otherParent) {
    // Handle IO nodes
    for (int i = 1; i <= INodes; i++) {
        auto newInputNode = std::make_shared<Node>(NodeType::INPUT, i);
        inputNodes.push_back(newInputNode);
        nodes[i] = newInputNode;
    }
    for (int i = 1; i <= ONodes; i++) {
        auto newOutputNode = std::make_shared<Node>(NodeType::OUTPUT, i+INodes);
        outputNodes.push_back(newOutputNode);
        nodes[i+INodes] = newOutputNode;
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

            // Handle mutation("if disabled in either parent, 75% it is disabled")
            if (!fitGenes[i].isEnabled || !otherGenes[j].isEnabled) {
                constexpr double probMutDis = 0.75;
                if (getRandom(0,1) < probMutDis) {connectionGenes.back().disable();}
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
    mutate();

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
    std::vector<double> output;
    for (const auto& nodeGene : outputNodes) {
        output.push_back(nodeGene->getOutput());
    }
    resetNetwork();
    return output;
}

void Genome::setInputs(const std::vector<double> &input) {
    for (int i = 0; i < inputNodes.size(); i++) {
        inputNodes[i]->setOutput(input[i]);
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
            nodes.insert({synapse._start, std::make_shared<Node>(NodeType::HIDDEN, synapse._start)});
        }
        if (!nodes.contains(synapse._end)) {
            nodes.insert({synapse._end, std::make_shared<Node>(NodeType::HIDDEN, synapse._end)});
        }

        nodes[synapse._end]->addPrevious(nodes[synapse._start], synapse._weight);
    }
}

bool Genome::compareHistorical(Synapse& synapse1, Synapse& synapse2) {
    return synapse1.historicalNumber < synapse2.historicalNumber;
}

double Genome::compareSimilarity(Genome &genome) {
    const auto selfGenome = getConnectionGenes();
    const auto otherGenome = genome.getConnectionGenes();

    const auto N = static_cast<int>(std::max(selfGenome.size(), otherGenome.size()));

    int excess = 0, disjoint = 0, equal = 0;
    double weightDifference = 0.0;

    int i = 0, j = 0;

    while (i < selfGenome.size() && j < otherGenome.size()) {
        if (selfGenome[i].historicalNumber == otherGenome[j].historicalNumber) {
            weightDifference += std::abs(selfGenome[i]._weight - otherGenome[j]._weight);
            i++; j++; equal++;
            continue;
        }
        if (selfGenome[i].historicalNumber < otherGenome[j].historicalNumber) i++;
        else {j++;}
        disjoint++;
    }

    while (i < selfGenome.size()) {excess++; i++;};
    while (j < otherGenome.size()) {excess++; j++;}

    return c1*excess/N+c1*disjoint/N+c3*weightDifference/equal;
}





void Genome::mutateWeight() {
    constexpr double perturbeOverRandom = 0.9;

    if (getRandom(0, 1) < perturbeOverRandom) {
        perturbeWeight();
    }
    else randomWeight();
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

    // Give up if connection is disabled
    if (!connectionGenes[synapseId].isEnabled) {return;}

    const auto start = connectionGenes[synapseId]._start;
    const auto end = connectionGenes[synapseId]._end;

    const int newNodeNumber = getNodeNumber()+getIONodes();
    // const auto newNode = std::make_shared<Node>(NodeType::HIDDEN, newNodeNumber);
    // nodes[newNodeNumber] = newNode;

    setupNewNode(start, newNodeNumber, 1);
    setupNewNode(newNodeNumber, end, connectionGenes[synapseId]._weight);
    connectionGenes[synapseId].disable();
}

void Genome::setupNewNode(int start, int end, double weight) {
    auto historicalNumber = getInnovation();
    connectionGenes.emplace_back(start, end, weight, historicalNumber);
    synapseMap[std::pair(start,end)] = historicalNumber;
}

void Genome::mutateConnection() {
    const auto nodes = topSort();

    const int startNodeId = static_cast<int>(getRandom(0,nodes.size()-ONodes));
    const int endNodeId = static_cast<int>(getRandom(std::max(INodes, startNodeId),nodes.size()-ONodes));

    // Check if mutation already exists
    if (synapseMap.contains(std::pair(nodes[startNodeId], nodes[endNodeId]))) {
        // Check if genome has mutation
        const int historicalNumber = synapseMap[std::pair(nodes[startNodeId], nodes[endNodeId])];
        for (auto &synapse : connectionGenes) {
            // If exists, enable and give up on adding
            if (synapse.historicalNumber == historicalNumber) {synapse.enable(); return;}
        }
        // If it doesn't add with old historicalNumber
        connectionGenes.emplace_back(nodes[startNodeId], nodes[endNodeId], getRandom(), historicalNumber);
    }
    // It's a new mutation:
    else {
        const auto historicalNumber = getInnovation();
        connectionGenes.emplace_back(nodes[startNodeId], nodes[endNodeId], getRandom(), historicalNumber);
        synapseMap[std::pair(nodes[startNodeId], nodes[endNodeId])] = historicalNumber;
    }
}

void Genome::mutate() {
    constexpr double probMutWeight = 0.8;
    constexpr double probMutConn = 0.05;
    constexpr double probMutNode = 0.03;

    if (getRandom(0,1) < probMutWeight) mutateWeight();
    if (getRandom(0,1) < probMutConn) mutateConnection();
    if (getRandom(0,1) < probMutNode) mutateNode();
}

// Thought-up implementation, normally it's DFS(postorder)
std::vector<int> Genome::topSort() {
    std::unordered_set<int> seen;
    std::vector<int> helper;
    std::deque<std::shared_ptr<Node>> queue;
    std::vector<int> result;

    // setup BFS
    for (auto& node : outputNodes) {
        queue.push_back(node);
        helper.push_back(node->getNumber());
    }

    // BFS through nodes
    while (!queue.empty()) {
        const auto node = queue.front();
        queue.pop_front();
        for (auto& previous : node->getPrevious()) {
            queue.push_back(previous);
            helper.push_back(previous->getNumber());
        }
    }

    // Create output vector
    for (int i = helper.size() - 1; i >= 0; i--) {
        if (!seen.contains(helper[i])) {
            seen.insert(helper[i]);
            result.push_back(helper[i]);
        }
    }

    return result;
}











