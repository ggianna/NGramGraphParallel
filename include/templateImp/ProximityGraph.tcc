/**
 * \file ProximityGraph.tcc
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


template <typename PayloadType, typename AtomType>
ProximityGraph<PayloadType, AtomType>::ProximityGraph()
{
    evaluator = nullptr;
    splitter = nullptr;
    payload = nullptr;
}



template <typename PayloadType, typename AtomType>
ProximityGraph<PayloadType, AtomType>::ProximityGraph(ProximityEvaluator<AtomType> *newEvaluator,
		Splitter<PayloadType, AtomType> *newSplitter, Payload<PayloadType> *newPayload)
{
    evaluator = newEvaluator;
    splitter = newSplitter;
    payload = newPayload;
}



template <typename PayloadType, typename AtomType>
ProximityGraph<PayloadType, AtomType>::~ProximityGraph()
{
    //dtor
}



template <typename PayloadType, typename AtomType>
void ProximityGraph<PayloadType, AtomType>::createGraph()
{
    std::vector<Atom<std::string>> atoms;
    typename Graph(std::string)::vertex_descriptor currentVertex;

    if (splitter == nullptr)
    {
        // TODO : throw exception for missing splitter
        std::cerr << "ProximityGraph::createGraph() : Error: missing splitter. Exiting..." << std::endl; return;
    }
    // split payload to get the n-grams (atoms)
    atoms = splitter->splitPayloadToAtoms(payload);

    // add the atoms to the graph
    for (typename std::vector<Atom<std::string>>::iterator itCurrentAtom = atoms.begin() ; itCurrentAtom != atoms.end() ; ++itCurrentAtom)
    {
        // add or locate current vertex (atom) in the graph
        currentVertex = this->addVertex(*itCurrentAtom);

        // add an edge from current vertex (atom) to every other vertex (atom) it should connect to, based on the proximity evaluator
        for (typename std::vector<Atom<std::string>>::iterator itPrecedingAtom = atoms.begin() ; itPrecedingAtom != itCurrentAtom ; ++itPrecedingAtom)
        {
            if (evaluator->areClose(*itCurrentAtom, *itPrecedingAtom))
                this->addEdge(currentVertex, this->UniqueVertices[*itPrecedingAtom], EDGE_WEIGHT_DEFAULT_VALUE);
        }
    }
}

template <typename PayloadType, typename AtomType>
void ProximityGraph<PayloadType, AtomType>::addEdge(typename Graph(AtomType)::vertex_descriptor vHead,
		typename Graph(AtomType)::vertex_descriptor vTail, EDGE_WEIGHT_TYPE edgeWeight)
{
    std::string edgeName;
    std::pair<typename Graph(AtomType)::edge_descriptor, bool> edge = boost::edge(vHead, vTail, this->graph);
    //check if edge exists
    if (edge.second == false) // edge doesn't exist, so add it
    {
        EdgeWeightProperty ewp = edgeWeight;
        boost::add_edge(vHead, vTail, ewp, this->graph);
	edgeName = this->graph[vHead].toString() + "___" + this->graph[vTail].toString();
	edgeNameToWeightMap[edgeName] = edgeWeight;

    }
    else    // edge.second == true, edge exists, so update its weight
    {
	edgeName = this->graph[vHead].toString() + "___" + this->graph[vTail].toString();
        this->edgeWeightMap[edge.first] += edgeWeight;
	edgeNameToWeightMap[edgeName] += edgeWeight;
    }
}

template <typename PayloadType, typename AtomType>
void ProximityGraph<PayloadType, AtomType>::addEdge(Atom<AtomType> aHead,
		Atom<AtomType> aTail, EDGE_WEIGHT_TYPE edgeWeight)
{
    typename Graph(AtomType)::vertex_descriptor vHead, vTail;
    std::string edgeName;

    // Locate the vertices
    // Since addVertex() will check if the vertex already exists, a simple call will return the descriptor to the (either new or found) vertex we want.
    vHead = this->addVertex(aHead);
    vTail = this->addVertex(aTail);

    // vertices located, search if edge already exists
    std::pair<typename Graph(AtomType)::edge_descriptor, bool> edge = boost::edge(vHead, vTail, this->graph);
    //check if edge exists
//    std::cout << "Exists: " << edge.second << std::endl;    // DEBUG
    if (edge.second == false) // edge doesn't exist, so add it
    {
        EdgeWeightProperty ewp = edgeWeight;
        boost::add_edge(vHead, vTail, ewp, this->graph);
	edgeName = aHead.toString() + "___" + aTail.toString();
	edgeNameToWeightMap[edgeName] = edgeWeight;
//        std::cout << "Exists: " << boost::edge(vHead, vTail, graph).second << " with weight " << boost::get( boost::edge_weight, graph, boost::edge(vHead, vTail, graph).first ) << std::endl;  // DEBUG
    }
    else    // edge.second == true, edge exists, so update its weight
    {
	edgeName = aHead.toString() + "___" + aTail.toString();
        this->edgeWeightMap[edge.first] += edgeWeight;
	edgeNameToWeightMap[edgeName] += edgeWeight;
//        std::cout << "Exists: " << boost::edge(vHead, vTail, graph).second << " with weight " << boost::get( boost::edge_weight, graph, boost::edge(vHead, vTail, graph).first ) << std::endl;  // DEBUG
    }
}

template <typename PayloadType, typename AtomType>
EDGE_WEIGHT_TYPE ProximityGraph<PayloadType, AtomType>::getEdgeWeightByName(std::string edgeName)
{
	std::unordered_map<std::string, EDGE_WEIGHT_TYPE>::const_iterator got = edgeNameToWeightMap.find(edgeName);
	if (got == edgeNameToWeightMap.end()) //Edge name not found
		return 0;
	else
		return got->second;
}

template <typename PayloadType, typename AtomType>
void ProximityGraph<PayloadType, AtomType>::updateEdgeWeight(Atom<AtomType> aHead,
		Atom<AtomType> aTail, EDGE_WEIGHT_TYPE edgeWeight)
{

    typename Graph(AtomType)::vertex_descriptor vHead, vTail;
    std::string edgeName = aHead.toString() + "___" + aTail.toString();

    // Locate the vertices
    // Since addVertex() will check if the vertex already exists, a simple call will return the descriptor to the (either new or found) vertex we want.
    vHead = this->addVertex(aHead);
    vTail = this->addVertex(aTail);

    // vertices located, search if edge already exists
    std::pair<typename Graph(AtomType)::edge_descriptor, bool> edge = boost::edge(vHead, vTail, this->graph);
    //check if edge exists
    if (edge.second == false) // edge doesn't exist
    {
        EdgeWeightProperty ewp = edgeWeight;
        boost::add_edge(vHead, vTail, ewp, this->graph);
	edgeNameToWeightMap[edgeName] = edgeWeight;
	std::cout << std::endl << "ProximityGraph::updateEdgeWeight(): BUG(Edge not present in the graph)" << std::endl;
	//TODO Throw EdgeNotPresentException
    }
    else    // edge exists, so update its weight
    {
        this->edgeWeightMap[edge.first] = edgeWeight;
	edgeNameToWeightMap[edgeName] = edgeWeight;
    }
}
