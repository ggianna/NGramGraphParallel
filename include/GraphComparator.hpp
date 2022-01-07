/**
 * \file GraphComparator.hpp
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

#ifndef GRAPHCOMPARATOR_H
#define GRAPHCOMPARATOR_H

// project headers
#include "GraphSimilarity.hpp"
#include "ProximityGraph.hpp"


/**
 * \Class
 * A class that compares ProximityGraphs. It is type-agnostic, so the types are templated.
 */
template <typename PayloadType, typename AtomType>
class GraphComparator
{
    public:
        /** Default constructor */
        GraphComparator();



        /** Default destructor */
        virtual ~GraphComparator();



        /** Compares two ProximityGraphs. The result is returned in the special class designed for this reason, GraphSimilarity.
	 * This function is NOT symmetric, e.g. compare(pGraph1, pGraph2) != compare(pGraph2, pGraph1). This is because the
	 * value containment operator is not symmetric.
         * \param pGraph1 a reference to a ProximityGraph
         * \param pGraph2 a reference to a ProximityGraph
         * \return The similarity between the two graphs, in GraphSimilarity format.
         */
        virtual GraphSimilarity compare(ProximityGraph<PayloadType, AtomType> &pGraph1, ProximityGraph<PayloadType, AtomType> &pGraph2);
		virtual double calculateContainmentSimilarity(ProximityGraph<PayloadType, AtomType> &pGraph1, ProximityGraph<PayloadType, AtomType> &pGraph2, std::string option);


    protected:

	/*
	 * Calculates the value ratio scaling factor between two ProximityGraph objects.
	 * \param pGraph1 A reference to the first ProximityGraph object.
	 * \param pGraph2 A reference to the seconde ProximityGraph object.
	 * \return The value ratio scaling factor.
	 */
	virtual double calculateValueRatio(ProximityGraph<PayloadType, AtomType> &pGraph1, ProximityGraph<PayloadType, AtomType> &pGraph2);


	/*
	 * Calculates the ratio of two edge weights, smaller to bigger.
	 * \param w1 The first edge weight.
	 * \param w2 The second edge weight.
	 * \return Their ratio, smaller to bigger.
	 */
	virtual double minMaxRatio(double w1, double w2);


};


// template implementation file
#include "templateImp/GraphComparator.tcc"


#endif // GRAPHCOMPARATOR_H
