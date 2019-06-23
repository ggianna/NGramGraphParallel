#include "DocumentClass.hpp"

DocumentClass::DocumentClass() : NGramGraph(), numberOfConstituents(0), updateOp(this, nullptr, 1.0) {}


void DocumentClass::update(NGramGraph *newDoc) {
	numberOfConstituents += 1;
	double new_l = 1 - ( (numberOfConstituents - 1) / double(numberOfConstituents) );
	updateOp.setLearningFactor(new_l);
	updateOp.setDocumentOperand(newDoc);
	updateOp.apply();
}


void DocumentClass::update(NGramGraph *newDoc, double l) {
	numberOfConstituents += 1;
	updateOp.setLearningFactor(l);
	updateOp.setDocumentOperand(newDoc);
	updateOp.apply();
}
