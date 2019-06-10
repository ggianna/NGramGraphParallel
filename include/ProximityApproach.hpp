/**
 * \file ProximityApproach.hpp
 *
 * \author Vasilis Politis
 *
 * \copyright Copyright 2019 NCSR Demokritos Greece \n
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


#ifndef PROXIMITY_APPROACH_H
#define PROXIMITY_APPROACH_H

#include "NGramGraph.hpp"


enum class ProximityApproachType { NonSymmetric, Symmetric };

/**
 * \Class An abstract class that describes the fact that different proximity approaches can be used to create the n-gram graph, e.g. symmetric,
 * non-symmetric etc.
 * Usage: Every NGramGraph object has a ProximityApproach attached to it, that uses when creating the graph.
 */
class ProximityApproach
{
	public:
		/* Default destructor */
		virtual ~ProximityApproach() {}


		/**
		 * Creates the n-gram graph, by splitting the initial payload into atoms and connecting them with edges.
		 * \param g The NGramGraph object whose payload will be used to create the n-gram graph.
		 */
		virtual void createGraph(NGramGraph *g) = 0;


		/**
		 * Creates edges between a single vertex and every other vertex of a vector.
		 * The edges' direction (in, out or both in and out) and weight is subclass-dependent.
		 * if an edge already exists, it's weight get's updated.
		 * \param v The signle vertex that participates in all edges.
		 * \param neighbors The vector of vertices each of which will be connected to the single vertex v.
		 */
		virtual void createEdgesToNeighbors( Graph(std::string)::vertex_descriptor v,
				std::vector<typename Graph(std::string)::vertex_descriptor> neighbors, NGramGraph *g ) = 0;
};


class SymmetricApproach : public ProximityApproach
{
	public:
		/* Default constructor */
		SymmetricApproach();


		/* Default destructor */
		virtual ~SymmetricApproach();


		/**
		 * Creates the n-gram graph using the symmetric approach, where the window is centered at the n-gram of interest, taking into account
		 * both preceding and succeeding n-grams.
		 */
		virtual void createGraph(NGramGraph *g);


		/**
		 * The vertices are connected in both directions. The edges' weight is distance-independent.
		 * Example: for v=1 and neighbors=[2,3], we create/update the edges: 1->2, 2->1, 1->3, 3->1, using the same weight for all.
		 */
		virtual void createEdgesToNeighbors( Graph(std::string)::vertex_descriptor v,
				std::vector<typename Graph(std::string)::vertex_descriptor> neighbors, NGramGraph *g );
};	



class NonSymmetricApproach : public ProximityApproach
{
	public:
		/* default constructor */
		NonSymmetricApproach();

		
		/* default destructor */
		virtual ~NonSymmetricApproach();


		/**
		 * Creates the n-gram graph using the non-symmetric approach. If the n-gram of interest is located at position p, the window spans from p-D,
		 * to p-1, where D is the window length, taking into account only preceding n-grams.
		 */
		virtual void createGraph(NGramGraph *g);


		/**
		 * The single vertex is the source and the vertices of the vector the destination of the edges. The edges' weight is distance-independent.
		 * Example: for v=1 and neighbors=[2,3], we create/update the edges: 1->2, 1->3, using the same weight for all.
		 */
		virtual void createEdgesToNeighbors( Graph(std::string)::vertex_descriptor v,
				std::vector<typename Graph(std::string)::vertex_descriptor> neighbors, NGramGraph *g );
};


#endif //PROXIMITY_APPROACH_H
