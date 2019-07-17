#include "DocumentClass.hpp"


DocumentClass::DocumentClass() : NGramGraph(), numberOfConstituents(0), updateOp(this, nullptr, 1.0) {}


void DocumentClass::update(NGramGraph *newDoc, bool precise) {
	updateOp.setPrecise(precise);
	numberOfConstituents += 1;
	float new_l = computeLearningFactor();
	updateOp.setLearningFactor(new_l);
	updateOp.setDocumentOperand(newDoc);
	updateOp.apply();
}


void DocumentClass::update(NGramGraph *newDoc, double l, bool precise) {
	updateOp.setPrecise(precise);
	numberOfConstituents += 1;
	updateOp.setLearningFactor(l);
	updateOp.setDocumentOperand(newDoc);
	updateOp.apply();
}


float DocumentClass::computeLearningFactor() {
	return ( 1 - ( (numberOfConstituents - 1) / float(numberOfConstituents) ) );
}
