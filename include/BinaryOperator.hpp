#ifndef BINARYOPERATOR_H
#define BINARYOPERATOR_H

#include "Operator.hpp"

template <typename OperandType, typename ValueType>
class BinaryOperator : public Operator<OperandType, ValueType> {
protected:

	/*
	 * Constructor accesible to derived classes.
	 * \param operand1 Pointer to the first operand.
	 * \param operand2 Pointer to the second operand.
	 */
	BinaryOperator(OperandType *operand1, OperandType *operand2) : Operator<OperandType, ValueType>(2) {
		Operator<OperandType, ValueType>::operands[0] = operand1;
		Operator<OperandType, ValueType>::operands[1] = operand2;
	}

public:

	/*
	 * Sets the operands.
	 * \papam operand1 The first operand.
	 * \param operand2 The second operand.
	 */
	void setOperands(OperandType *operand1, OperandType *operand2) {
		Operator<OperandType, ValueType>::operands[0] = operand1;
		Operator<OperandType, ValueType>::operands[1] = operand2;
	}


	/*
	 * Sets the first operand.
	 * \param openrad1 The new first operand.
	 */
	void setFirstOperand(OperandType *operand1) {
		Operator<OperandType, ValueType>::operands[0] = operand1;
	}


	/*
	 * Sets the seconds operand.
	 * \param operand2 The new second operand.
	 */
	void setSecondOperand(OperandType *operand2) {
		Operator<OperandType, ValueType>::operands[1] = operand2;
	}


	virtual ValueType apply() = 0;

};

#endif // BINARYOPERATOR_H
