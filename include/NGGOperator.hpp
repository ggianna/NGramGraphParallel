#ifndef NGGOPERATOR_H
#define NGGOPERATOR_H

#include "NGramGraph.hpp"

/*
 * \class An operator that applies on NGramGraphs
 */
class NGGOperator {
protected:

	/*
	 * Pointers to the operands.
	 */
	vector<NGramGraph *> operands;


	/*
	 * Constructor accesible to derived classes.
	 */
	NGGOperator(unsigned int arity) : operands(arity) {}

public:

	/*
	 * Applies the operator to the operands.
	 * \return The result of the operation.
	 */
	virtual NGramGraph apply() = 0;
};

/*
 * \class A unary NGGOperator
 */
class UnaryNGGOperator : public NGGOperator {
public:

	/*
	 * Constructor
	 */
	UnaryNGGOperator(NGramGraph *operand1) : NGGOperator(1) {
		operands[0] = operand1;
	}
	
	/*
	 * Sets the operand.
	 * \papam operand1 The new operand.
	 */
	void setOperands(NGramGraph *operand1) {
		operands.clear();
		operands[0] = operand1;
	}
};

/*
 * \class A binary NGGOperator
 */
class BinaryNGGOperator : public NGGOperator {
protected:

	/*
	 * Constructor accesible to derived classes.
	 */
	BinaryNGGOperator(NGramGraph *operand1, NGramGraph *operand2) : NGGOperator(2) {
		operands[0] = operand1;
		operands[1] = operand2;
	}
	

public:

	/*
	 * Sets the operands.
	 * \papam operand1 The first operand.
	 * \param operand2 The second operand.
	 */
	void setOperands(NGramGraph *operand1, NGramGraph *operand2) {
		operands.clear();
		operands[0] = operand1;
		operands[1] = operand2;
	}
};

class NGGMergeOperator : public BinaryNGGOperator {
public:

	/*
	 * Constructor
	 */
	NGGMergeOperator(NGramGraph *operand1, NGramGraph *operand2);

	/*
	 * Implementation of the merge operation.
	 * \return The NGramGraph that results after merging the operands.
	 */
	NGramGraph apply() override;

protected:

	/*
	 * Utility function to find the small and the big graph among the operands w.r.t. the number of edges.
	 */
	std::pair<NGramGraph *, NGramGraph *> findSmallAndBigGraphs();

	/*
	 * Utility function to break an edge label to it's constituents,
	 * e.g. the string representations of the head and the tail atoms.
	 */
	std::pair<std::string, std::string> extractHeadAndTailFromEdgeLabel(std::string label);
};

#endif //NGGOPERATOR_H
