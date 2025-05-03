#ifndef NODE_H
#define NODE_H

#include "node_type.h"
#include <vector>
#include <Eigen/Dense>
#include <memory>
#include <cmath>


class Node {
public:
    Node(const NodeType type, const int nodeNumber);
    ~Node() = default;
    double getOutput();
    void addPrevious(const std::shared_ptr<Node> &previous, double weight);
    void reset();

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
