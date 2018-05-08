/**
 * \file DivisibleObject.h
 */

#ifndef DIVISIBLEOBJECT_H
#define DIVISIBLEOBJECT_H

#include <vector>

/**
 * \Class A class that represents an object that can be split into Atoms.
 * Divisible objects are the entity that a N-gram graph holds. The graph splits the DivisibleObject to atoms, and creates a node for every atom created.
 */
template <class DivisibleObjectType>
class DivisibleObject
{
    public:
        /**
         * Constructor for DivisibleObject class.
         * \param object The original object that will be split into Atoms.
         *
         */
        DivisibleObject(DivisibleObjectType object);

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
/*
        /**
         * Assignment operator for DivisibleObject class.
         * \param other The DivisibleObject which will be copied.
         *
         */
        DivisibleObject& operator=(const DivisibleObject& other);
*/
        /**
         * Compares the DivisibleObject with another DivisibleObject instance using the usual '==' operator
         * \param rhDivisibleObject A reference to a DivisibleObject instance to be compared with.
         * \return A boolean value: True if the two DivisibleObjects are identical, otherwise False
         *
         */
        bool operator==(const DivisibleObject& rhDivisibleObject);

        /** \brief Accessor for payload variable
         * \return The DivisibleObject's payload
         */
        DivisibleObjectType getPayload(){ return payload; }

        /** \brief Accessor for atoms variable
         * \return The DivisibleObject's atoms vector
         */
        vector<Atom> getAtoms(){ return atoms; }

    protected:
        /** \var payload
         * Payload is the original object that the DivisibleObject instance holds, that can be split into Atoms
         */
        DivisibleObjectType payload;

        /** \var atoms
         * A vector that holds the smallest pieces (atoms) of payload after it has been split.
         */
        vector<Atom> atoms;

        //TODO implement splitPayloadToAtoms() as a public or protected function?
        /**
         * Splits the DivisibleObject's payload to Atoms, filling the atoms vector
         * \note Must be overwritten class-based to do the splitting according to the class used
         */
        void splitPayloadToAtoms();
};

#include "DivisibleObject.tcc"

#endif // DIVISIBLEOBJECT_H
