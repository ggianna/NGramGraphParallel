#ifndef DOCUMENTCLASS_H
#define DOCUMENTCLASS_H

#include "NGramGraph.hpp"
#include "NGGUpdateOperator.hpp"

/* 
 * \class An NGramGraph-derived class that represents a class of documents.
 */
class DocumentClass : public NGramGraph {
protected:

	/*
	 * The number of documents that constitue the class.
	 */
	unsigned int numberOfConstituents;


	/*
	 * An NGGUpdateOperator to come in handy when the document class is updated.
	 */
	NGGUpdateOperator updateOp;

public:
	/*
	 * Constructor.
	 * Payload, Splitter and ProximityEvaluator member fields are all set to NULL and never used.
	 */
	DocumentClass();


	/*
	 * Updates the document class with a new document.
	 * \param newDoc Pointer to the new document of the class.
	 */
	void update(NGramGraph *newDoc);


	/*
	 * Updates the document class with a new document, using a specific learning factor.
	 * \param newDoc Pointer to the new document of the class.
	 * \param l The learning factor to be used.
	 */
	void update(NGramGraph *newDoc, double l);
};

#endif // DOCUMENTCLASS_H
