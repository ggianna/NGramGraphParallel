#include "NGGMergeOperator.hpp"

std::pair<NGramGraph *, NGramGraph *> NGGMergeOperator::findSmallAndBigGraphs() {
	short bigIndex = ( operands[0]->numberOfEdges() > operands[1]->numberOfEdges() ) ? 0 : 1;
	short smallIndex = 1 - bigIndex;

	return { operands[smallIndex], operands[bigIndex] };
}

NGramGraph NGGMergeOperator::apply() {
	// Find the small and the big graph
	NGramGraph *small, *big;
	std::tie(small, big) = findSmallAndBigGraphs();

	// Clone the big graph
	NGramGraph mergedGraph(*big);

	// Create an update operator with the cloned big graph as the first operand, and learning factor 0.5
	NGGUpdateOperator updateOp(&mergedGraph, small, 0.5);

	updateOp.apply();

	// The mergedGraph is now ready
	return mergedGraph;
}
