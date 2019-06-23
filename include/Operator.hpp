#ifndef OPERATOR_H
#define OPERATOR_H

#include <vector>

/*
 * \class An operator that applies to it's operands of type OperandType,
 * producing a value of type ValueType.
 */
template <typename OperandType, typename ValueType>
class Operator {
protected:

	/*
	 * Pointers to the operands.
	 */
	std::vector<OperandType *> operands;

	/*
	 * Construct accesible to derived classes.
	 */
	Operator(unsigned int arity) : operands(arity) {}

public:

	/*
	 * Applies the operator to the operands.
	 * \return The result of the operation.
	 */
	virtual ValueType apply() = 0;
};

#endif // OPERATOR_H
