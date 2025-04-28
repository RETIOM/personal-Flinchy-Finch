//
// Created by tommeh on 24.04.25.
//

#ifndef GENOME_H
#define GENOME_H

#include "node.h"
#include "synapse.h"

#include <optional>
#include <unordered_map>
#include <memory>
#include <algorithm>

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

    static double getRandom(float min = -2.0, float max = 2.0);
    inline static int innovationNumber = 1;
    static int getInnovation() {return innovationNumber++;};

    inline static int hiddenNodeNumber = 1;
    static int getNodeNumber() {return hiddenNodeNumber++;}

    static std::unordered_map<std::pair<int,int>, int> synapseMap;

    inline static int INodes;
    inline static int ONodes;
    static int getIONodes() {return INodes+ONodes;}
    static void setIONodes(int inputs, int outputs) {INodes=inputs; ONodes=outputs;};

    std::vector<Synapse>& getConnectionGenes() {std::sort(connectionGenes.begin(), connectionGenes.end(), compareHistorical); return connectionGenes;};
    void addConnection(int start, int end, double weight);

    double compareSimilarity(Genome& genome);
private:
    // std::vector<Node> nodeGenes;
    std::unordered_map<int, std::shared_ptr<Node>> nodes;
    std::vector<Synapse> connectionGenes;

    std::vector<Node> inputNodes;
    std::vector<Node> outputNodes;


    // Crossover functions
    static bool compareHistorical(Synapse& synapse1, Synapse& synapse2);

    // Speciation functions
    const double c1 = 1.0;
    const double c2 = 1.0;
    const double c3 = 0.4;



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
    void setInputs(const std::vector<double> &input);

};



#endif //GENOME_H
