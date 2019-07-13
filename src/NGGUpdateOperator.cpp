#include "NGGUpdateOperator.hpp"

std::pair<std::string, std::string> NGGUpdateOperator::extractHeadAndTailFromEdgeLabel(std::string label) {
	std::string separator = EDGE_LABEL_SEPARATOR;
	size_t pos = label.find(separator);
	/*
	 * if (pos == std::string:npos)
	 * 	throw BadEdgeLabelSeparator;
	 */
	std::string head = label.substr(0, pos);
	std::string tail = label.substr(pos + separator.length());

	return { head, tail };
}


void NGGUpdateOperator::apply() {
	NGramGraph *classGraph = operands[0];
	NGramGraph *newDoc = operands[1];

	std::string label, head, tail;
	Atom<std::string> aHead, aTail;
	EDGE_WEIGHT_TYPE docWeight, classWeight, newWeight;
	for (auto const& elem : newDoc->getEdgeNameToWeightMap()) {
		label = elem.first;
		docWeight = elem.second;
		classWeight = classGraph->getEdgeWeightByName(label);
		newWeight = classWeight + (docWeight - classWeight) * learningFactor;
		std::tie(head, tail) = extractHeadAndTailFromEdgeLabel(label);
		aHead.setData(head);
		aTail.setData(tail);
		if (classWeight == 0) { // The edge was not originally present on the class graph
			classGraph->addEdge(aHead, aTail, newWeight);
		}
		else { //The edge was already present on the class graph
			classGraph->updateEdgeWeight(aHead, aTail, newWeight);
		}
	}

	if (precise) {
		for (auto const& elem : classGraph->getEdgeNameToWeightMap()) {
			label = elem.first;
			classWeight = elem.second;
			if (newDoc->getEdgeWeightByName(label) == 0) { // The edge is not present on the document graph
				newWeight = classWeight * (1 - learningFactor);
				std::tie(head, tail) = extractHeadAndTailFromEdgeLabel(label);
				aHead.setData(head);
				aTail.setData(tail);
				classGraph->updateEdgeWeight(aHead, aTail, newWeight);
			}
		}
	}
}
