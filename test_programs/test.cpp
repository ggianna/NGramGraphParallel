/**
 * \file main.cpp
 *
 * \author Panagiotis Fotopoulos
 *
 * \copyright Copyright 2018 NCSR Demokritos Greece \n
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. \n
 */

#include <iostream>
#include "GraphSimilarity.hpp"
#include "GraphComparator.hpp"
#include "NGramGraph.hpp"
#include "UniqueVertexGraph.hpp"
#include "StringAtom.hpp"
#include "StringSplitter.hpp"
#include "ProximityApproach.hpp"
#include "DocumentClass.hpp"
#include "NGGMergeOperator.hpp"

#define TEXT_PAYLOAD "Hello World"

using namespace std;

int main(){

    // An example of graph creation:

    // Declare a splitter for the Proximity Graph (string splitter here, since we will split a text to n-grams).
    StringSplitter testStringSplitter(4);

    // Set the size of the atoms (n-grams in this  case). Default is 3.
	//testStringSplitter.setAtomSize(NGRAMSIZE_DEFAULT_VALUE);

    // Declare the payload of the Proximity Graph (the entity that will be split to atoms). We are using a text in this example, so it will be a string payload.
    StringPayload testStringPayload(TEXT_PAYLOAD);

    // Choose the symmetric proximity approach.
    ProximityApproach* approach = new SymmetricApproach();

    // Declare the Proximity Graph. We will split a text into n-grams, so we are using an NGramGraph which is a ProximityGraph for this use.
    // The NGramGraph has a built-in basic Proximity Evaluator so we won't explicitly pass one, but it needs a String Splitter and a String Payload to split.
    NGramGraph testNGramGraph(nullptr, &testStringSplitter, &testStringPayload, 2, approach);

    GraphSimilarity testGraphSimilarity;
    NGramGraph halfgraph(nullptr, &testStringSplitter, &testStringPayload, 2, approach);

    cout << "created halfgraph" << endl;

    // Create the graph from the text (StringPayload) and print it in DOT format.
    testNGramGraph.createGraph();
    halfgraph.createGraph();

    testNGramGraph.printGraphviz();
    testNGramGraph.printGraphvizToFile("out.dot");
	

    DocumentClass docClass;
    // update docClass (which is empty) with testNGramGraph (the graph produced from TEXT_PAYLOAD)
    docClass.update(&testNGramGraph);
    docClass.update(&halfgraph);

    // because docClass was initially empty, docClass and testNGramGraph should be the same now
    if (docClass == testNGramGraph) {
        std::cout << std::endl << "update method of DocumentClass OK" << std::endl;
    }

    // merge testNGramGraph with itself and get the result in a new NGramGraph object
    
    
    NGGMergeOperator mergeOp(&testNGramGraph, &testNGramGraph);
    NGramGraph opValue = mergeOp.apply();

    GraphComparator<std::string, std::string> testGraphComparator;
    testGraphSimilarity = testGraphComparator.compare(docClass, halfgraph);

    map<string, double> testSimilarityComponents =  testGraphSimilarity.getSimilarityComponents();
    cout << "Size Similarity: " << testSimilarityComponents["sizeSimilarity"] << endl;
    cout << "Value Similarity: " << testSimilarityComponents["valueSimilarity"] << endl;
    cout << "Normalized Value Similarity: " << testSimilarityComponents["normalizedValueSimilarity"] << endl;
    cout << "Containment Value Similarity: " << testSimilarityComponents["containmentSimilarity"] << endl;

    // the result of the merge operation should be the same with testNGramGraph
    if (opValue == testNGramGraph)
	    std::cout << std::endl << "merge operator OK" <<std::endl;

    // -------------------------------------------------------------------------------------------
    cout << endl << endl << "-------------------------" << endl << endl << endl;


    
    // An example of graph comparison:

    // Declare an instance of GraphSimilarity to hold the comparison's result

    // Declare the graph comparator

    // Compare the previous graph with itself and store the result to testGraphSimilarity
    testGraphSimilarity = testGraphComparator.compare(testNGramGraph, testNGramGraph);

    // Display all defined similarities of the two compared graphs.
    cout << "Size Similarity: " << testSimilarityComponents["sizeSimilarity"] << endl;
    cout << "Value Similarity: " << testSimilarityComponents["valueSimilarity"] << endl;
    cout << "Normalized Value Similarity: " << testSimilarityComponents["normalizedValueSimilarity"] << endl;
    cout << "Containment Value Similarity: " << testSimilarityComponents["containmentSimilarity"] << endl;
    // Since they are the same graph, all similarities should be equal to 1.
    

    return 0;

}
