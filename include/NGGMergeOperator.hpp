#ifndef NGGMERGEOPERATOR_H
#define NGGMERGEOPERATOR_H

#include "BinaryOperator.hpp"
#include "NGGUpdateOperator.hpp"
#include "NGramGraph.hpp"

//TODO Add an NGGUpdateOperator object as a member field, and set it's operands in the apply().

/*
 * \class Implements the merge operator for NGramGraph objects.
 * The operator doesn't alter it's operands, but creates a new NGramGraph object to hold the result.
 */
class NGGMergeOperator : public BinaryOperator<NGramGraph, NGramGraph> {
public:

	/*
	 * Constructor
	 * \param operand1 Pointer to the first NGramGraph operand.
	 * \param operand2 Pointer to the second NGramGraph operand.
	 */
	NGGMergeOperator(NGramGraph *operand1, NGramGraph *operand2) : BinaryOperator(operand1, operand2) {}

	/*
	 * Implementation of the merge operator.
	 * \return The new merged NGramGraph object.
	 */
	NGramGraph apply() override;

private:

	/*
	 * Utility function to find the small and the big graph among the operands w.r.t. the number of edges.
	 * \return A pair of NGramGraph pointers, in ascending order w.r.t. number of edges.
	 */
	std::pair<NGramGraph *, NGramGraph *> findSmallAndBigGraphs();
};

#endif // NGGMERGEOPERATOR_H
