//
// Created by tommeh on 24.04.25.
//

#include "../include/node.h"


void Node::addPrevious(const std::shared_ptr<Node> &previous, double weight) {
    _previous.push_back(previous);
    weights.resize(_previous.size());
    weights(_previous.size()-1) = weight;
}


double Node::getOutput() {
    if (wasCalled) {
        return output;
    }
    inputs.resize(_previous.size());

    // iterate through previous, put their getOuptu into input matrix
    for (int i = 0; i < _previous.size(); i++) {
      inputs(i) = _previous[i]->getOutput();
    }

    output = activationFunction(inputs.dot(weights));
    output = 1.0;
    _type != NodeType::OUTPUT ? wasCalled = true : wasCalled = false;
    return output;
}

void Node::setOutput(double input) {
	if (_type == NodeType::INPUT) {
          output = input;
	}
}


