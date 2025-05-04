#include "../include/genome.h"
#include <random>
#include "../include/utils.h"

#include <iostream>

Genome::Genome(int inputNumber, int outputNumber) {
    nodes[0] = std::make_shared<Node>(NodeType::BIAS, 0);

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
    // Insert bias Node
    nodes[0] = std::make_shared<Node>(NodeType::BIAS, 0);

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
            if (static_cast<int>(utils::getRandom(0,2)) == 1)
                connectionGenes.push_back(fitGenes[i]);
            else connectionGenes.push_back(otherGenes[j]);

            // Handle mutation("if disabled in either parent, 75% it is disabled")
            if (!fitGenes[i].isEnabled || !otherGenes[j].isEnabled) {
                constexpr double probMutDis = 0.75;
                if (utils::getRandom(0,1) < probMutDis) {connectionGenes.back().disable();}
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


std::vector<double> Genome::getOutput(const std::vector<double> &input) {
    setInputs(input);
    std::vector<double> output;
    for (const auto& nodeGene : outputNodes) {
        output.push_back(nodeGene->getOutput());
    }
    resetNetwork();
    return output;
}

void Genome::setInputs(const std::vector<double> &input) const {
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



    // Genomes without connections are always the same(edge case)
    if (selfGenome.size() == 0 && otherGenome.size() == 0) {return 0;}

    int excess = 0, disjoint = 0, equal = 0;
    double weightDifference = 0.0;


    auto N = static_cast<int>(std::max(selfGenome.size(), otherGenome.size()));

    if (N < 20) N = 1;

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

    auto output = c1*excess/N+c2*disjoint/N;

    if (std::min(selfGenome.size(), otherGenome.size()) > 0) {
        output += c3*weightDifference/equal;
    }

    return output;
}





void Genome::mutateWeight() {
    constexpr double perturbeOverRandom = 0.9;

    if (utils::getRandom(0, 1) < perturbeOverRandom) {
        perturbeWeight();
    }
    else randomWeight();
}

void Genome::randomWeight() {
    const auto id = static_cast<int>(utils::getRandom(0,connectionGenes.size()));
    connectionGenes[id].changeWeight(utils::getRandom());
}

void Genome::perturbeWeight() {
    const auto id = static_cast<int>(utils::getRandom(0,connectionGenes.size()));
    connectionGenes[id].perturbeWeight(utils::getRandom(0,2));
}

void Genome::mutateNode() {
    // Create a vector of viable connections
    std::vector<Synapse*> viableConnections;

    for (auto& synapse : connectionGenes) {
        if (synapse.isEnabled && synapse._start != 0)
            viableConnections.push_back(&synapse);
    }

    if (viableConnections.empty()) {return;}

    const auto synapseId = static_cast<int>(utils::getRandom(0,viableConnections.size()));

    const auto start = viableConnections[synapseId]->_start;
    const auto end = viableConnections[synapseId]->_end;
    const auto weight = viableConnections[synapseId]->_weight;
    viableConnections[synapseId]->disable();

    const int newNodeNumber = getNodeNumber() + getIONodes();

    setupNewNode(start, newNodeNumber, 1);
    setupNewNode(newNodeNumber, end, weight);
}

void Genome::setupNewNode(int start, int end, double weight) {
    auto historicalNumber = getInnovation();
    connectionGenes.emplace_back(start, end, weight, historicalNumber);
    synapseMap[std::pair(start,end)] = historicalNumber;
}

void Genome::mutateConnection() {
    auto sortedNodes = topSort();

    const int startNodeId = static_cast<int>(utils::getRandom(0,sortedNodes.size()-ONodes));
    const int endNodeId = static_cast<int>(utils::getRandom(std::max(INodes+1, startNodeId+1),sortedNodes.size()));

    auto newPair = std::make_pair(sortedNodes[startNodeId], sortedNodes[endNodeId]);
    // Check if mutation already exists
    if (synapseMap.contains(newPair)) {
        // Check if genome has mutation
        const int historicalNumber = synapseMap[std::pair(sortedNodes[startNodeId], sortedNodes[endNodeId])];
        for (auto &synapse : connectionGenes) {
            // If exists, enable and give up on adding
            if (synapse.historicalNumber == historicalNumber) {synapse.enable(); return;}
        }
        // If it doesn't add with old historicalNumber
        connectionGenes.emplace_back(sortedNodes[startNodeId], sortedNodes[endNodeId], utils::getRandom(), historicalNumber);
    }
    // It's a new mutation:
    else {
        const auto historicalNumber = getInnovation();
        connectionGenes.emplace_back(sortedNodes[startNodeId], sortedNodes[endNodeId], utils::getRandom(), historicalNumber);
        synapseMap[std::pair(sortedNodes[startNodeId], sortedNodes[endNodeId])] = historicalNumber;
    }
}

void Genome::enableConnection() {
    // Choose a random disabled connection and enable it
    std::vector<Synapse*> disabledConnections;
    for (auto& synapse : connectionGenes) {
        if (!synapse.isEnabled) {disabledConnections.push_back(&synapse);}
    }

    if (disabledConnections.empty()) {return;}

    auto connectionToDisable = static_cast<int>(utils::getRandom(0,disabledConnections.size()));
    disabledConnections[connectionToDisable]->enable();
}

void Genome::disableConnection() {
    // Choose a random enabled connection and disable it
    std::vector<Synapse*> enabledConnections;
    for (auto& synapse : connectionGenes) {
        if (synapse.isEnabled) {enabledConnections.push_back(&synapse);}
    }

    if (enabledConnections.empty()) {return;}

    auto connectionToDisable = static_cast<int>(utils::getRandom(0,enabledConnections.size()));
    enabledConnections[connectionToDisable]->disable();
}

void Genome::mutate() {
    constexpr double probMutWeight = 0.8;
    constexpr double probMutConn = 0.05;
    constexpr double probMutNode = 0.03;
    constexpr double probDisConn = 0.2;
    constexpr double probEnConn = 0.1;

    if (!connectionGenes.empty()) {
        if (utils::getRandom(0,1) < probMutNode) mutateNode();
        if (utils::getRandom(0,1) < probMutWeight) mutateWeight();
        if (utils::getRandom(0,1) < probEnConn) enableConnection();
        if (utils::getRandom(0,1) < probDisConn) disableConnection();
    }

    if (utils::getRandom(0,1) < probMutConn) mutateConnection();

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

    // Make sure inputs and bias are always included
    for (int i = INodes; i >= 0; i--) {
        helper.push_back(i);
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