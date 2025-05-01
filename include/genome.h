//
// Created by tommeh on 24.04.25.
//

#ifndef GENOME_H
#define GENOME_H

#include "node.h"
#include "synapse.h"

#include <optional>
#include <unordered_map>
#include <unordered_set>
#include <deque>
#include <memory>
#include <algorithm>
#include <utility>

/* this holds the agents genome: node and connection(synapse) genes as ?vectors?
 * it needs a (public) getOutput method which will calculate the NN output
 * TODO: Figure out how to crossover and stuff(where to do the selection): selection above, crossover in the fitter parent
 * private method to prepareNetwork (get weights and connections to nodes)
*/

class Genome {
public:

    Genome(int inputNumber, int outputNumber);
    // Constructor used for Crossover:
    Genome(Genome& fitterParent, Genome& otherParent);
    ~Genome() = default;

    // Computing network
    std::vector<double> getOutput(const std::vector<double> &input);

    inline static int innovationNumber = 1;
    static int getInnovation() {return innovationNumber++;};

    inline static int hiddenNodeNumber = 1;
    static int getNodeNumber() {return hiddenNodeNumber++;}

    // Hash function for std::pair<int><int>
    struct PairHash {
        std::size_t operator()(const std::pair<int, int>& p) const {
            // Get hash values for both components
            std::size_t h1 = std::hash<int>{}(p.first);
            std::size_t h2 = std::hash<int>{}(p.second);

            // Combine the hash values using bitwise operations
            return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
        }
    };

    inline static std::unordered_map<std::pair<int,int>, int, PairHash> synapseMap;
    static bool checkSynapseMap(std::pair<int,int> synapse) {return synapseMap.contains(synapse);};
    static int getSynapseNumber(std::pair<int,int> synapse) {return synapseMap[synapse];};
    static void addSynapseNumber(std::pair<int,int> synapse, int historicalNumber) {synapseMap[synapse] = historicalNumber;};

    inline static int INodes;
    inline static int ONodes;
    static int getIONodes() {return INodes+ONodes;}
    static void setIONodes(int inputs, int outputs) {INodes=inputs; ONodes=outputs;};

    std::vector<Synapse>& getConnectionGenes() {std::sort(connectionGenes.begin(), connectionGenes.end(), compareHistorical); return connectionGenes;};
    void setupNewNode(int start, int end, double weight);

    double compareSimilarity(Genome& genome);

    void runMutate() {mutate();};

    // Functions for testing
    void addConnection(Synapse& synapse) {connectionGenes.push_back(synapse);};
    void runInferNetwork() {inferNetwork();}
    std::unordered_map<int, std::shared_ptr<Node>> getNodes() {return nodes;};
    std::vector<Synapse>& getConnections() {return connectionGenes;};
    void runSetInputs(const std::vector<double> &inputs) {setInputs(inputs);}
    std::vector<std::shared_ptr<Node>>& getInputNodes() {return inputNodes;};
    std::vector<std::shared_ptr<Node>>& getOutputNodes() {return outputNodes;};
    std::vector<int> runTopSort() {return topSort();};

private:
    std::unordered_map<int, std::shared_ptr<Node>> nodes;
    std::vector<Synapse> connectionGenes;

    std::vector<std::shared_ptr<Node>> inputNodes;
    std::vector<std::shared_ptr<Node>> outputNodes;


    // Crossover functions
    static bool compareHistorical(Synapse& synapse1, Synapse& synapse2);

    // Speciation functions
    const double c1 = 1.0;
    const double c2 = 1.0;
    const double c3 = 0.4;

    // Topological sort
    std::vector<int> topSort();

    // Mutation functions
    void mutate(); // Mutation wrapper
    void mutateConnection(); // Add connection
    void mutateNode(); // Add node in existing connection
    // mutateEnabled handled by crossover constructor
    void mutateWeight();
    void perturbeWeight(); // Multiply between [0,2]
    void randomWeight(); // In the name

    // Computing a Network
    void inferNetwork();
    void resetNetwork();
    void setInputs(const std::vector<double> &input) const;

};



#endif //GENOME_H
