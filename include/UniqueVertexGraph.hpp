/**
 * \file UniqueVertexGraph.hpp
 */

#ifndef UNIQUEVERTEXGRAPH_H
#define UNIQUEVERTEXGRAPH_H

// system headers
#include <boost/graph/directed_graph.hpp>
#include <boost/graph/graphviz.hpp>
#include <unordered_map>

// project headers
#include "Atom.hpp"


// defines
#define EDGE_WEIGHT_TYPE double
#define Graph(AtomType) boost::directed_graph<Atom<AtomType>,EdgeWeightProperty>


// typedefs
typedef boost::property<boost::edge_weight_t, EDGE_WEIGHT_TYPE> EdgeWeightProperty;


/**
 * \Class A class that represents a weighted graph with unique vertices.
 *
 */
template <typename AtomType>
class UniqueVertexGraph
{

    public:

        // graph and property map typedefs for readabilitiness
        typedef typename boost::property_map<Graph(AtomType), boost::edge_weight_t>::type EdgeWeightMap;



        /**
         * Constructor for the UniqueVertexGraph class.
         */
        UniqueVertexGraph();



        /** Default destructor */
        virtual ~UniqueVertexGraph();



//        // DEPRECATED: THE OTHER addVertex() FUNCTION SHOULD BE USED.
//        /** Adds a new vertex to the graph. If a vertex holding the same Atom already exists, it doesn't add a new one, but instead throws an Exception.
//         * \param aAtom The Atom that will be added to the graph.
//         * \throws TryingToAddExistingVertexException
//         */
//        void addVertex(Atom<AtomType> aAtom);



        /** Adds a new vertex to the graph, and returns its vertex descriptor.
         * If a vertex holding the same Atom already exists, it doesn't add a new one, but instead returns the vertex descriptor of the vertex that holds the Atom.
         * \param aAtom The Atom that will be added to the graph.
         * \return The vertex descriptor of the (newly added or found) vertex that holds the aAtom Atom.
         */
        typename Graph(AtomType)::vertex_descriptor addVertex(Atom<AtomType> aAtom);



        /** Removes a vertex from the graph, based on the Atom it's holding.
//         * Also removes all the edges it's connected to.
         * \param aAtom The Atom of the vertex that will be removed from the graph.
         */
        void removeVertex(Atom<AtomType> aAtom);



        /** Removes a vertex from the graph, based on its vertex descriptor.
         * Also removes all the edges it's connected to.
         * \param vVertex The vertex descriptor of the vertex that will be removed from the graph.
         */
        void removeVertex(typename Graph(AtomType)::vertex_descriptor vVertex);



        /** Checks whether a given vertex exists in this graph.
         * \param aAtom The Atom that we are searching for in the graph.
         * \return True if the Atom exists in the graph, otherwise false.
         */
        bool contains(Atom<AtomType> aAtom);



        /** Adds a new edge to the graph, directed from vHead to vTail.
         * If an edge connecting the same Atoms already exists, it doesn't add a new one, but instead updates the weight of the edge.
         * If any vertex is missing, it will be added to the graph before the insertion of the edge.
         * \param vHead The vertex descriptor of the Atom that is the head of the edge that will be added to the graph.
         * \param vTail The vertex descriptor of the Atom that is the tail of the edge that will be added to the graph.
         * \param edgeWeight The weight of the edge that will be added/updated to the graph.
         */
        void addEdge(typename Graph(AtomType)::vertex_descriptor vHead, typename Graph(AtomType)::vertex_descriptor vTail, EDGE_WEIGHT_TYPE edgeWeight);



        /** Adds a new edge to the graph, directed from aHead to aTail.
         * If an edge connecting the same Atoms already exists, it doesn't add a new one, but instead updates the weight of the edge.
         * If any vertex is missing, it will be added to the graph before the insertion of the edge.
         * \param aHead The Atom that is the head of the edge that will be added to the graph.
         * \param aTail The Atom that is the tail of the edge that will be added to the graph.
         * \param edgeWeight The weight of the edge that will be added/updated to the graph.
         */
        void addEdge(Atom<AtomType> aHead, Atom<AtomType> aTail, EDGE_WEIGHT_TYPE edgeWeight);



    protected:

        /**
         * \var graph The graph of the UniqueVertexGraph.
         * Implemented with Boost Graph Library as a directed graph with weighted edges.
         */
        Graph(AtomType) graph;



        /**
         * \var UniqueVertices A hashtable containing pairs <Atom, Vertex> used to uniquely identify each vertex.
         */
        std::unordered_map<Atom<AtomType>, typename Graph(AtomType)::vertex_descriptor> UniqueVertices;



        /**
         * \var edgeWeightMap A (boost::property_map) map used to retrieve the weight of the edges.
         */
        EdgeWeightMap edgeWeightMap;



        /** Removes a vertex from the graph. Also removes all the edges it's connected to. \n
         * This is the function that actually removes vertices. The other 2 public functions are wrappers for this one. \n
         * Takes both types that a vertex can be identified with, to avoid extra function calls.
         * \param aAtom The Atom of the vertex that will be removed from the graph.
         * \param vVertex The vertex descriptor of the vertex that will be removed from the graph.
         */
        void removeVertex(Atom<AtomType> aAtom, typename Graph(AtomType)::vertex_descriptor vVertex);
};


// template implementation file
#include "UniqueVertexGraph.tcc"


#endif // UNIQUEVERTEXGRAPH_H
