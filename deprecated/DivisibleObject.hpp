/**
 * \file DivisibleObject.hpp
 */

#ifndef DIVISIBLEOBJECT_H
#define DIVISIBLEOBJECT_H

#include "Atom.hpp"
#include <vector>

using namespace std;


/**
 * \Class A class that represents an object that can be split into Atoms.
 * Divisible objects are the entity that a N-gram graph holds. The graph splits the DivisibleObject to atoms, and creates a node for every atom created.
 */
template <typename AtomType>
class DivisibleObject
{
    public:
        /**
         * Constructor for DivisibleObject class.
         * \param object The original object that will be split into Atoms.
         *
         */
        DivisibleObject();



        /**
         * Copy constructor for DivisibleObject class.
         * \param sourceDivisibleObject The original DivisibleObject instance whose attributes will be copied to the newly created one.
         *
         */
        DivisibleObject(const DivisibleObject& sourceDivisibleObject);



        /**
         * Destructor for DivisibleObject class.
         *
         */
        virtual ~DivisibleObject();



        /**
         * Assignment operator for DivisibleObject class.
         * \param other The DivisibleObject which will be copied.
         *
         */
        //DivisibleObject& operator=(const DivisibleObject& other);



        /**
         * Compares the DivisibleObject with another DivisibleObject instance using the usual '==' operator
         * \param rhDivisibleObject A reference to a DivisibleObject instance to be compared with.
         * \return A boolean value: True if the two DivisibleObjects are identical, otherwise False
         *
         */
        //bool operator==(const DivisibleObject& rhDivisibleObject);


        void setPayload(AtomType p){ this->payload = p; }


        /** \brief Accessor for payload variable
         * \return The DivisibleObject's payload
         */
        AtomType getPayload(){ return payload; }



        /** \brief Accessor for atoms variable
         * \return The DivisibleObject's atoms vector
         */
        vector<Atom<AtomType>> getAtoms(){ return atoms; }



        /**
         * Provide a readable representation (string) of the data that the DivisibleObject holds
         * \return Returns a string made by the payload
         */
        std::string toString();



        /**
         * Provide a readable representation (string) of the data that the DivisibleObject's Atoms hold
         * \return Returns a vector with strings made by the string representation of each Atom
         */
        vector<std::string> atomsToString();



        /**
         * Splits the DivisibleObject's payload to Atoms with AtomSize size, filling the atoms vector
         * \param AtomSize The size of each Atom.
         * \note Must be type-based overwritten (template specialization) to do the splitting according to the type used.
         */
        void splitPayloadToAtoms(unsigned int AtomSize);



    protected:
        /** \var payload
         * Payload is the original object that the DivisibleObject instance holds, that can be split into Atoms
         */
        AtomType payload;



        /** \var atoms
         * A vector that holds the smallest pieces (atoms) of payload after it has been split.
         */
        vector<Atom<AtomType>> atoms;
};


#include "DivisibleObject.tcc"


#endif // DIVISIBLEOBJECT_H