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

    static double getRandom();

    ~Genome() = default;
    std::vector<double> getOutput(std::vector<double> input);

private:
    std::vector<Node> nodeGenes;
    std::vector<Synapse> connectionGenes;

    std::vector<Node*> inputNodes;
    std::vector<Node*> outputNodes;

    void prepareNetwork();
    void resetNetwork();
    void setInputs(std::vector<double> &input);

};



#endif //GENOME_H
