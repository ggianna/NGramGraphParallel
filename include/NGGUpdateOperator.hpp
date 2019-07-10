#ifndef NGG_UPDATE_OPERATOR_H
#define NGG_UPDATE_OPERATOR_H

#include "BinaryOperator.hpp"
#include "NGramGraph.hpp"
#include "StringAtom.hpp"

/*
 * \class Implements the update operator for NGramGraph objects.
 * The result of the operation is stored within the first operand, which is the class graph that gets updated.
 * As a result, the ValueType of this operator is void.
 */
class NGGUpdateOperator : public BinaryOperator<NGramGraph, void> {
private:

	/*
	 * The learning factor to be used while applying the update operator.
	 */
	float learningFactor;


public:

	/*
	 * Constructor
	 * \param classGraph Pointer the class n-gram graph to be updated.
	 * \param newDoc Pointer to the new document n-gram graph that will join the class.
	 * \param l The learning factor to be used in the update operation.
	 */
	NGGUpdateOperator(NGramGraph *classGraph, NGramGraph *newDoc, double l) : BinaryOperator(classGraph, newDoc), learningFactor(l) {}


	/*
	 * Implementation of the update operator.
	 */
	void apply() override;


	/*
	 * Sets the learning factor.
	 * \param l The new learning factor.
	 */
	void setLearningFactor(float l) { learningFactor = l; }


	/*
	 * Sets the class operand, the one that actually gets updated.
	 * \param docClass Pointer to the new operand.
	 */
	void setClassOperand(NGramGraph *docClass) { setFirstOperand(docClass); }


	/*
	 * Sets the document operand, the new document to join the class.
	 * \param doc Pointer to the new operand.
	 */
	void setDocumentOperand(NGramGraph *doc) { setSecondOperand(doc); }


	/*
	 * Utility function to break an edge label to it's constituents,
	 * e.g. the string representations of the head and tail atoms.
	 * \param label The edge's label.
	 * \return The string constituents of the label, as a pair of strings.
	 */
	std::pair<std::string, std::string> extractHeadAndTailFromEdgeLabel(std::string label);
};

#endif // NGGUPDATEOPERATOR_H
