//
// Created by tommeh on 24.04.25.
//

#ifndef NODE_H
#define NODE_H

#include "node_type.h"
#include <vector>
#include <Eigen/Dense>
#include <memory>

/* Node will be made up of
 * Attributes:
 * - TYPE
 * - bias
 * - vector of input nodes and/or values
 * - vector of weights
 * - activation function
 * - output
 * - wasCalled bool
 * - reference to global innovation number
 * Methods:
 * - getOutput
*/

// generate owen random bias

class Node {
public:
    Node(const NodeType type = NodeType::HIDDEN): _type(type){}
    ~Node() = default;
    double getOutput();
    void addPrevious(const std::shared_ptr<Node> &previous, double weight);
    void reset() {_type != NodeType::INPUT ? wasCalled = false : wasCalled = true;}

    void setOutput(double output);

private:
    NodeType _type;
    std::vector<std::shared_ptr<Node>> _previous;

    // weights
    Eigen::VectorXd weights;
    // inputs
    Eigen::VectorXd inputs;

    double output = 0;
    bool wasCalled = false;

    double activationFunction(double interimOutput);
};


#endif //NODE_H
