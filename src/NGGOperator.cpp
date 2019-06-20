#include "NGGOperator.hpp"
#include "NGramGraph.hpp"
#include "StringAtom.hpp"

NGGMergeOperator::NGGMergeOperator(NGramGraph *operand1, NGramGraph *operand2) : BinaryNGGOperator(operand1, operand2) {}

std::pair<std::string, std::string> NGGMergeOperator::extractHeadAndTailFromEdgeLabel(std::string label) {
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

std::pair<NGramGraph *, NGramGraph *> NGGMergeOperator::findSmallAndBigGraphs() {
	short bigIndex = ( operands[0]->numberOfEdges() > operands[1]->numberOfEdges() ) ? 0 : 1;
	short smallIndex = 1 - bigIndex;

	return { operands[smallIndex], operands[bigIndex] };
}

NGramGraph NGGMergeOperator::apply() {
	//find the small and the big graph
	NGramGraph *small, *big;
	std::tie(small, big) = findSmallAndBigGraphs();
	

	//clone the big graph
	NGramGraph mergedNGG(*big);
	

	//iterate over the edges of the small graph. If an edge already exists in the cloned big graph, update it's weight.
	//Else insert it.
	std::string label, head, tail;
	EDGE_WEIGHT_TYPE weight1, weight2, averageWeight;
	Atom<std::string> aHead, aTail;
	for (auto const& elem : small->getEdgeNameToWeightMap()) {
		label = elem.first;
		weight1 = elem.second;
		std::tie(head, tail) = extractHeadAndTailFromEdgeLabel(label);
		aHead.setData(head);
		aTail.setData(tail);
		if ( (weight2 = mergedNGG.getEdgeWeightByName(label)) == 0 ) { //The edge was not found on the big graph
			averageWeight = weight1 / 2.0;
			mergedNGG.addEdge(aHead, aTail, averageWeight);
		}
		else { //The edge is present on the big graph
			averageWeight = (weight1 + weight2) / 2.0;
			mergedNGG.updateEdgeWeight(aHead, aTail, averageWeight);
		}
	}

	return mergedNGG;
}
