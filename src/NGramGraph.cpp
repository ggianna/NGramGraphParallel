/**
 * \file NGramGraph.cpp
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

#include "NGramGraph.hpp"
#include "ProximityApproach.hpp"
#include <boost/graph/graph_utility.hpp>



NGramGraph::NGramGraph()
{
    CorrelationWindow = CORRELATIONWINDOW_DEFAULT_VALUE;
    splitter = nullptr;
    evaluator = nullptr;
    payload = nullptr;
}



NGramGraph::NGramGraph(ProximityEvaluator<std::string> *newProximityEvaluator, StringSplitter *newSplitter, StringPayload *newPayload, unsigned int CorrelationWindowValue, ProximityApproach* proximityApproach)
{
    evaluator = newProximityEvaluator;
    splitter = newSplitter;
    payload = newPayload;
    CorrelationWindow = CorrelationWindowValue;
    approach = proximityApproach;

}



NGramGraph::~NGramGraph()
{
    //dtor
}



void NGramGraph::createGraph()
{
    approach->createGraph(this);
}


bool NGramGraph::operator==(const NGramGraph& other)
{
	if (numberOfEdges() != other.numberOfEdges()) {
		return false;
	}

	bool ret = true;
	std::unordered_map<std::string, EDGE_WEIGHT_TYPE>::const_iterator got;
	for (auto const& elem : other.getEdgeNameToWeightMap()) {	
		got = edgeNameToWeightMap.find(elem.first);
		if (got == edgeNameToWeightMap.end()) { // Edge name not found.
			ret = false;
			break;
		}
		else if (got->second != elem.second) { //Edge name found but with different weight.
			// TODO Check float inequality in a better way
			ret = false;
			break;
		}
	}

	return ret;
}









