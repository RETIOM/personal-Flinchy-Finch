//
// Created by tommeh on 24.04.25.
//

#ifndef NODE_H
#define NODE_H

#include "node_type.h"
#include <vector>
#include <Eigen/Dense>
#include <memory>
#include <cmath>

/* Node will be made up of
 * Attributes:
 * - TYPE
 * - vector of input nodes and/or values
 * - vector of weights
 * - activation function
 * - output
 * - wasCalled bool
 * Methods:
 * - getOutput
*/

// generate owen random bias

class Node {
public:
    Node(const NodeType type, const int nodeNumber): _type(type), _nodeNumber(nodeNumber) { _type == NodeType::INPUT ? wasCalled = true : false; }
    ~Node() = default;
    double getOutput();
    void addPrevious(const std::shared_ptr<Node> &previous, double weight);
    void reset() {_type != NodeType::INPUT ? wasCalled = false : wasCalled = true;}

    void setOutput(double output);

    int getNumber() {return _nodeNumber;}

    // Testing
    Eigen::VectorXd& getWeights() {return weights;}
    bool getWasCalled() {return wasCalled;};
    NodeType getType() {return _type;};
    std::vector<std::shared_ptr<Node>> getPrevious() {return _previous;};
    Eigen::VectorXd& getInputs() {return inputs;};

private:
    NodeType _type;
    std::vector<std::shared_ptr<Node>> _previous;

    // weights
    Eigen::VectorXd weights;
    // inputs
    Eigen::VectorXd inputs;

    double output = 0;
    bool wasCalled = false;

    int _nodeNumber;

    static double activationFunction(double interimOutput) {return 1.0 / (1.0 + std::exp(-interimOutput));};


};


#endif //NODE_H
