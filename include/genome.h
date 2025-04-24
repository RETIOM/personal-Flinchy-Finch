//
// Created by tommeh on 24.04.25.
//

#ifndef GENOME_H
#define GENOME_H

#include "node.h"
#include "synapse.h"

#include <vector>

/* this holds the agents genome: node and connection(synapse) genes as ?vectors?
 * it needs a (public) getOutput method which will calculate the NN output
 * TODO: Figure out how to crossover and stuff(where to do the selection): selection above, crossover in the fitter parent
 * private method to prepareNetwork (get weights and connections to nodes)
*/

class Genome {
public:
    Genome(int inputNumber, int outputNumber);
    ~Genome() = default;
    double getOutput();

private:
    std::vector<Node> nodeGenes;
    std::vector<Synapse> connectionGenes;

    std::vector<Node*> inputNodes;
    Node* outputNode;

    void prepareNetwork();
    void resetNetwork();

};



#endif //GENOME_H
