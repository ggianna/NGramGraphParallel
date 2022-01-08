/**
 * \file GraphComparator.tcc
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
GraphComparator<PayloadType, AtomType>::GraphComparator()
{
//    similarity = new GraphSimilarity();
}

template <typename PayloadType, typename AtomType>
GraphComparator<PayloadType, AtomType>::~GraphComparator()
{
//    delete similarity;
}


template <typename PayloadType, typename AtomType>
double GraphComparator<PayloadType, AtomType>::calculateValueRatio(ProximityGraph<PayloadType, AtomType> &pGraph1,
							ProximityGraph<PayloadType, AtomType> &pGraph2)
{
    double weight1, weight2, sum = 0;
    unsigned int numberOfEdges1 = pGraph1.numberOfEdges();
    unsigned int numberOfEdges2 = pGraph2.numberOfEdges();
    std::string name;
    ProximityGraph<PayloadType, AtomType> &smallGraph = (numberOfEdges1 < numberOfEdges2)  ? pGraph1 : pGraph2;
    ProximityGraph<PayloadType, AtomType> &bigGraph   = (numberOfEdges1 >= numberOfEdges2) ? pGraph1 : pGraph2;


    if ( (numberOfEdges1 > 0) && (numberOfEdges2 > 0) )
    {
        for ( auto const& pair : smallGraph.getEdgeNameToWeightMap() ) {
            name = pair.first;
	    weight1 = pair.second;
	    weight2 = bigGraph.getEdgeWeightByName(name);
	    if (weight2 > 0) {
	       	sum += minMaxRatio(weight1, weight2);
	    }
	}
    }

    return sum;
}


template <typename PayloadType, typename AtomType>
GraphSimilarity GraphComparator<PayloadType, AtomType>::compare(ProximityGraph<PayloadType, AtomType> &pGraph1,
							ProximityGraph<PayloadType, AtomType> &pGraph2)
{
    double valueSimilarity, containmentSimilarity, sizeSimilarity, normalizedValueSimilarity;
    unsigned int numberOfEdges1 = pGraph1.numberOfEdges();
    unsigned int numberOfEdges2 = pGraph2.numberOfEdges();
    unsigned int bigGraphEdges = std::max(numberOfEdges1, numberOfEdges2);
    unsigned int smallGraphEdges = std::min(numberOfEdges1, numberOfEdges2);

    double VR = calculateValueRatio(pGraph1, pGraph2);

    if (VR > 0) {
        sizeSimilarity = static_cast<double>(smallGraphEdges) / bigGraphEdges;
        valueSimilarity = VR / bigGraphEdges; 
        normalizedValueSimilarity = VR / smallGraphEdges;
        containmentSimilarity = VR / numberOfEdges1; // This makes the function assymetric w.r.t. it's arguments.
    }
    else {
        sizeSimilarity = (bigGraphEdges > 0) ? (static_cast<double>(smallGraphEdges) / bigGraphEdges) : 0.0;
	valueSimilarity = normalizedValueSimilarity = containmentSimilarity = 0.0;
    }


    GraphSimilarity gSimilarity(sizeSimilarity, valueSimilarity, normalizedValueSimilarity, containmentSimilarity);
    return gSimilarity;
}



template <typename PayloadType, typename AtomType>
double GraphComparator<PayloadType, AtomType>::calculateContainmentSimilarity(ProximityGraph<PayloadType, AtomType> &pGraph1,
							ProximityGraph<PayloadType, AtomType> &pGraph2, std::string option)
{
    unsigned int numberOfEdges1 = pGraph1.numberOfEdges();
    unsigned int numberOfEdges2 = pGraph2.numberOfEdges();
    double cs;
    double VR = calculateValueRatio(pGraph1, pGraph2);
    if(option == "CS" || option =="sqrtCS"){
        unsigned int bigGraphEdges = std::max(numberOfEdges1, numberOfEdges2);
        cs = VR / bigGraphEdges;
        if(option == "sqrtCS"){
            return sqrt(2-2*cs);
        }
        else{
            return cs;
        }
        
    }
    else if(option == "MinCS" || option =="sqrtMinCS"){
        unsigned int smallGraphEdges = std::min(numberOfEdges1, numberOfEdges2);
        cs = VR / smallGraphEdges;
        if(option == "sqrtCS"){
            return sqrt(2-2*cs);
        }
        else{
            return cs;
        }
    }
    else{
        assert(false && "Unsupported Containment Similarity Option");
    }
   
}

template <typename PayloadType, typename AtomType>
double GraphComparator<PayloadType, AtomType>::minMaxRatio(double w1, double w2)
{
    if (w1 > w2) {
        return w2 / w1;
    }
    else {
        return w1 / w2;
    }
}
