#include "../include/node.h"

Node::Node(const NodeType type, const int nodeNumber) :
_type(type),
_nodeNumber(nodeNumber) {
    _type == NodeType::INPUT ? wasCalled = true : false;
    switch (_type) {
        case NodeType::INPUT:
            wasCalled = true;
            break;
        case NodeType::BIAS:
            wasCalled = true;
            output = 1.0;
            break;
        default:
            wasCalled = false;
    }
}


void Node::addPrevious(const std::shared_ptr<Node> &previous, double weight) {
    _previous.push_back(previous);
    weights.conservativeResize(_previous.size());
    weights(_previous.size()-1) = weight;
}


double Node::getOutput() {
    if (wasCalled) {
        return output;
    }

    if ((_type == NodeType::HIDDEN || _type == NodeType::OUTPUT) && _previous.size() < 1) {
        wasCalled = true;
        output = 0;
        return output;
    }

    inputs.conservativeResize(_previous.size());

    // iterate through previous, put their getOuptut into input matrix
    for (int i = 0; i < _previous.size(); i++) {
        inputs(i) = _previous[i]->getOutput();
    }

    output = activationFunction(inputs.dot(weights.transpose()));

    wasCalled = true;
    return output;
}

void Node::setOutput(double input) {
	if (_type == NodeType::INPUT) {
          output = input;
	}
}

void Node::reset() {
    switch (_type) {
        case NodeType::INPUT:
            wasCalled = true;
            break;
        case NodeType::BIAS:
            wasCalled = true;
            break;
        default:
            wasCalled = false;
    }
}



