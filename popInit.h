/* 
 * File:   popInit.h
 * Author: MAN
 *
 * Created on September 7, 2013, 7:01 AM
 */

#ifndef _POPINIT_H
#define	_POPINIT_H

#include <eo>
#include "routeSet.h"
#include "inputData.h"

void popInit(eoPop< RouteSet<double> >& _pop, int popSize, int routeSetSize, eoEvalFunc< RouteSet<double> >& _eval)
{
    RouteSet<double> initRs;
    vector< Route<double> > initV;
    ifstream fin(INITIAL_ROUTE_SET);
    for (int iroute = 0; iroute < routeSetSize; iroute++)
    {
        Route<double> initR;
        vector<int> initNodeList(VERTICES_NO, 0);

        list<int> initL;
        int node;
        fin >> node;
        while (node != -1)
        {
            initL.push_back(node);
            initNodeList[node] = 1;
            fin >> node;
        }
        initR.setR(initL);
        initR.setNodeList(initNodeList);
        initR.fitness(1.0 / DS[initL.front()][initL.back()]); //fitness 1/ds
        initV.push_back(initR);
    }
    initRs.setRs(initV);
    _eval(initRs);

    //std::cout<<initRs<<std::endl; //comment after test

    for (int igeno = 0; igeno < popSize; igeno++)
    {
        _pop.push_back(initRs);
    }
}

#endif	/* _POPINIT_H */

