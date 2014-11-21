/* 
 * File:   routeSetMutation.h
 * Author: MAN
 *
 * Created on September 8, 2013, 9:02 PM
 */

#ifndef _ROUTESETMUTATION_H
#define	_ROUTESETMUTATION_H

#define FRONT 0
#define BACK 1

//#include <eoOp.h>
//#include<utils/eoRNG.h>

#include "route.h"
#include "inputData.h"

template<class GenotypeT>
int rouletteWheelForRoute(GenotypeT & _genotype, eoEvalFuncPtr< Route<double> >& _eval)
{
    std::vector <double> fits;
    double total = 0;
    for (int i = 0; i < _genotype.size(); i++)
    {
        _eval(_genotype[i]);
        total += _genotype[i].fitness();
        fits.push_back(_genotype[i].fitness());
    }
    return rng.roulette_wheel(fits, total);

}

int rouletteWheelForPath(int startIndex)
{
    std::vector <double> fits;
    double total = 0;
    for (int i = 0; i < VERTICES_NO; i++)
    {
        if (startIndex == i)
        {
            fits.push_back(0);
            continue;
        }
        total += DS[startIndex][i];
        fits.push_back(DS[startIndex][i]);
    }
    return rng.roulette_wheel(fits, total);
}

template<class GenotypeT>
class SmallMutation : public eoMonOp<GenotypeT>
{
public:
    /**
     * Ctor - no requirement
     */
    // START eventually add or modify the anyVariable argument

    SmallMutation(float _pDelete, eoEvalFuncPtr< Route<double> >& _eval) : pDelete(_pDelete), eval(_eval)
    {
        // START Code of Ctor of an SmallMutation object
        // END   Code of Ctor of an SmallMutation object
    }

    /// The class name. Used to display statistics

    std::string className() const
    {
        return "SmallMutation";
    }

    /**
     * modifies the parent
     * @param _genotype The parent genotype (will be modified)
     */
    bool operator()(GenotypeT & _genotype)
    {
        // START code for mutation of the _genotype object
        int mutIndex = rouletteWheelForRoute(_genotype, eval);
        Route<double>& mutRoute = _genotype[mutIndex];

        bool bad = _genotype.isTrouble();

        //  std::cout << mutRoute << std::endl;
        std::list<int>::iterator it[2], nIt;
        it[FRONT] = mutRoute.mutableR().begin();
        it[BACK] = mutRoute.mutableR().end();
        it[BACK]--; //points to last elem
        int selectedEnd = rng.flip();
        nIt = it[selectedEnd];
        int selectedNode = *it[selectedEnd];

        if (pDelete >= rng.uniform()) //delete a terminal
        {
            if (mutRoute.mutableR().size() < 3) //path is too small, no deletion just return
                return false;
            mutRoute.erase(it[selectedEnd]);
            mutRoute.nodeList[selectedNode] = 0;
        }
        else //append a terminal
        {
            vector<int> AdjListForSelected = AdjList[selectedNode];
            for (vector<int>::iterator iit = AdjListForSelected.begin(); iit != AdjListForSelected.end(); iit++)
            {
                if (mutRoute.nodeList[*iit] == 1)
                {
                    AdjListForSelected.erase(iit);
                    --iit;
                }
            }
            if (AdjListForSelected.size() == 0)
                return false;
            int neighbor;
            if (selectedEnd == FRONT)
            {
                nIt++;
                neighbor = *nIt;
            }
            else
            {
                nIt--;
                neighbor = *nIt;
                it[BACK]++; //again points to end
            }
            int toAddIndex;
            // do
            //  {
            toAddIndex = rng.random(AdjListForSelected.size());
            // }
            // while (AdjList[selectedNode][toAddIndex] == neighbor);
            mutRoute.insert(it[selectedEnd], AdjListForSelected[toAddIndex]);
            mutRoute.nodeList[AdjListForSelected[toAddIndex]] = 1;


        }

        if(bad != _genotype.isTrouble())
        {
            cout<<"bad/n";
        }
        mutRoute.invalidate();
        return true;
        // END code for mutation of the _genotype object
    }

private:
    // START Private data of an SmallMutation object
    //  varType anyVariable;		   // for example ...
    // END   Private data of an SmallMutation object
    float pDelete;
    eoEvalFuncPtr< Route<double> >& eval;
};

template<class GenotypeT>
class BigMutation : public eoMonOp<GenotypeT>
{
public:
    /**
     * Ctor - no requirement
     */
    // START eventually add or modify the anyVariable argument

    BigMutation(eoEvalFuncPtr< Route<double> >& _eval) : eval(_eval)
    //  BigMutation( varType  _anyVariable) : anyVariable(_anyVariable)
    // END eventually add or modify the anyVariable argument
    {

        // START Code of Ctor of an BigMutation object
        // END   Code of Ctor of an BigMutation object
    }

    /// The class name. Used to display statistics

    string className() const
    {
        return "BigMutation";
    }

    /**
     * modifies the parent
     * @param _genotype The parent genotype (will be modified)
     */
    bool operator()(GenotypeT & _genotype)
    {
        // START code for mutation of the _genotype object
        int mutIndex = rouletteWheelForRoute(_genotype, eval);
        Route<double>& mutRoute = _genotype[mutIndex];

        //std::cout<<_genotype[mutIndex]<<std::endl;

        int terminal[2];
        terminal[0] = mutRoute.mutableR().front();
        terminal[1] = mutRoute.mutableR().back();
        int startNode = terminal[rng.flip()];
        if(startNode > VERTICES_NO)
        {
            cout<<mutRoute<<endl;
        }
        int endNode = rouletteWheelForPath(startNode);
        mutRoute.setR(sPath[startNode][endNode]);
        vector<int> newNodeList(VERTICES_NO, 0);
        for (list<int>::iterator lit = sPath[startNode][endNode].begin(); lit != sPath[startNode][endNode].end(); lit++)
        {
            newNodeList[*lit] = 1;
        }
        mutRoute.setNodeList(newNodeList);

        mutRoute.invalidate();
        return true;
        // END code for mutation of the _genotype object
    }

private:
    // START Private data of an BigMutation object
    eoEvalFuncPtr< Route<double> >& eval; // END   Private data of an BigMutation object
};


#endif	/* _ROUTESETMUTATION_H */

