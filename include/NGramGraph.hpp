/**
 * \file NGramGraph.hpp
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

#ifndef NGRAMGRAPH_H
#define NGRAMGRAPH_H

// project headers
#include "ProximityGraph.hpp"
#include "StringSplitter.hpp"
#include "cereal/types/unordered_map.hpp"
#include "cereal/types/memory.hpp"
#include "cereal/archives/binary.hpp"

// defines
#define NGRAMSIZE_DEFAULT_VALUE 3
#define CORRELATIONWINDOW_DEFAULT_VALUE 4


#ifdef __cplusplus
extern "C"{
#endif


// forward declaration
class ProximityApproach; //We need forward declaration, because NGramGraph and ProximityApproach refer to each other.


/**
 * \Class A class that represents a N-gram graph.
 *
 * A N-gram graph is a ProximityGraph for strings that are split into N-grams.
 * The graph's Atoms are N-grams, and they are connected iff they are close enough (according to the CorrelationWindow) inside the string.
 */
class NGramGraph : public ProximityGraph<std::string, std::string>
{
    public:


        
        /**
         * \var CorrelationWindow The maximum distance of terms to be considered as correlated. Default is 4.
         *
         */
	unsigned int CorrelationWindow;


        /** Default constructor.
         * CorrelationWindow is set to CORRELATIONWINDOW_DEFAULT_VALUE.
         * NGramSize is set to NGRAMSIZE_DEFAULT_VALUE.
         */
        NGramGraph();



        /** Constructor that initializes the CorrelationWindow and NGramSize variables.
         *  \param CorrelationWindowValue The new value for the CorrelationWindow variable.
         *  \param NGramSizeValue The new value for the NGramSize variable.
         */
        NGramGraph(ProximityEvaluator<std::string> *newProximityEvaluator, StringSplitter *newSplitter, StringPayload *newPayload, unsigned int CorrelationWindowValue, ProximityApproach* proximityApproach);



        /** Default destructor */
        virtual ~NGramGraph();



        /** Accessor for CorrelationWindow variable.
         * \return The current value of the CorrelationWindow variable.
         */
        unsigned int getCorrelationWindow() { return CorrelationWindow; }



        /** Mutator for CorrelationWindow variable.
         * \param CorrelationWindowValue The new value for the CorrelationWindow variable.
         */
        void setCorrelationWindow(unsigned int CorrelationWindowValue) { CorrelationWindow = CorrelationWindowValue; }



        /**
         * Creates the graph used internally by the ProximityGraph, according to the CorrelationWindow.
         */
        void createGraph();


	/**
	 * Compares this NGramGraph object with another one.
	 * \param A reference to the NGramGraph object to compare against.
	 * \return A bool value, true if the two objects are considered equal, false otherwise.
	 */
	bool operator==(const NGramGraph& other);
    // NGramGraph operator=(const NGramGraph& other);


    protected:
	/**
	 * \var approach The approach used to calculate proximity between the atoms.
	 *
	 */
	ProximityApproach *approach;





	
};
#ifdef __cplusplus
}
#endif
#endif // NGRAMGRAPH_H
