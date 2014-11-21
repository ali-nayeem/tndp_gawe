/* 
 * File:   routeSetQuadCrossOver.h
 * Author: MAN
 *
 * Created on September 8, 2013, 8:36 AM
 */

#ifndef _ROUTESETQUADCROSSOVER_H
#define	_ROUTESETQUADCROSSOVER_H

//#include <eoOp.h>

//#include "routeSet.h"

/**
 *  Always write a comment in this format before class definition
 *  if you want the class to be documented by Doxygen
 *
 * THere is NO ASSUMPTION on the class GenoypeT.
 * In particular, it does not need to derive from EO
 */
template<class GenotypeT>
class RouteSetQuadCrossover : public eoQuadOp<GenotypeT>
{
public:
    /**
     * Ctor - no requirement
     */
    // START eventually add or modify the anyVariable argument

    RouteSetQuadCrossover(float _pSwap) : pSwap(_pSwap)
    //  RouteSetQuadCrossover( varType  _anyVariable) : anyVariable(_anyVariable)
    // END eventually add or modify the anyVariable argument
    {
        // START Code of Ctor of an eoOneMaxEvalFunc object
        // END   Code of Ctor of an eoOneMaxEvalFunc object
    }

    /// The class name. Used to display statistics

    std::string className() const
    {
        return "RouteSetQuadCross0ver";
    }

    /**
     * eoQuad crossover - modifies both parents
     * @param _genotype1 The first parent
     * @param _genotype2 The second parent
     */
    bool operator()(GenotypeT& _genotype1, GenotypeT& _genotype2)
    {
        bool modified(false);
        bool bad = _genotype1.isTrouble();
        bool bad2 = _genotype2.isTrouble();
        // START code for crossover of _genotype1 and _genotype2 objects
        for (int i = 0; i < _genotype1.size(); i++)
        {
            if (pSwap >= rng.uniform())
            {
                Route<double> temp = _genotype1[i];
                _genotype1[i] = _genotype2[i];
                _genotype2[i] = temp;
                modified = true;

            }
        }
        /** Requirement
         * if (at least one genotype has been modified) // no way to distinguish
         *     oneAtLeastIsModified = true;
         * else
         *     oneAtLeastIsModified = false;
         */
        if(bad != _genotype1.isTrouble() || bad2 != _genotype2.isTrouble())
        {
            std::cout<<"bad/n";
        }
        return modified;
        // END code for crossover of _genotype1 and _genotype2 objects
    }

private:
    // START Private data of an RouteSetQuadCrossover object
    //  varType anyVariable;		   // for example ...
    // END   Private data of an RouteSetQuadCrossover object
    float pSwap;
};

#endif	/* _ROUTESETQUADCROSSOVER_H */

