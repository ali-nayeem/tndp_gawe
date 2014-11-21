/* 
 * File:   routeSetEvalFunc.h
 * Author: MAN
 *
 * Created on September 7, 2013, 6:44 PM
 */

#ifndef _ROUTESETEVALFUNC_H
#define	_ROUTESETEVALFUNC_H

#include <queue>

#include "routeSet.h"
//#include "eoEvalFunc.h"
//#include "inputData.h"
struct node
{
	node()
	{
	}
	node(int v,int cost, int transfer, int route, vector <int> vn)
	{
		vertex = v;
		totalCost = cost;
		totalTransfer = transfer;
		lastRoute = route;
		visitedNodes = vn;
	}
	int vertex;
	int totalCost;
	int totalTransfer;
	int lastRoute;
	vector <int> visitedNodes;
};
template <class EOT>
class RouteSetEvalFunc : public eoEvalFunc<EOT>
{
public:
    /// Ctor - no requirement
    // START eventually add or modify the anyVariable argument

    RouteSetEvalFunc()
    //  RouteSetEvalFunc( varType  _anyVariable) : anyVariable(_anyVariable)
    // END eventually add or modify the anyVariable argument
    {
        // START Code of Ctor of an RouteSetEvalFunc object
        // END   Code of Ctor of an RouteSetEvalFunc object
    }

    /** Actually compute the fitness
     *
     * @param EOT & _eo the EO object to evaluate
     *                  it should stay templatized to be usable
     *                  with any fitness type
     */
    void operator()(EOT & _eo)
    {
        // test for invalid to avoid recomputing fitness of unmodified individuals
        if (_eo.invalid())
        {
            // START Code of computation of fitness of the RouteSet object
            vector< Route<double> > routeSet = _eo.Rs();
            int dist[VERTICES_NO][VERTICES_NO];
            int transfer[VERTICES_NO][VERTICES_NO];
            list <int> AdjacencyList[VERTICES_NO];
            list <int> routeList[VERTICES_NO][VERTICES_NO];
            for (int i = 0; i < VERTICES_NO; i++)
            {
                dist[i][i] = 0;
                for (int j = i + 1; j < VERTICES_NO; j++)
                {
                    dist[j][i] = dist[i][j] = INFINITY;
                }
            }
            for (int r = 0; r < routeSet.size(); r++)
            {
                list<int>::const_iterator next = routeSet[r].R().begin();
                list<int>::const_iterator it = next++;
                for (; next != routeSet[r].R().end(); it++, next++)
                {
                    int i = *it;
                    int j = *next;
                    dist[i][j] = dist[j][i] = tr[i][j];
                    AdjacencyList[i].push_back(j);
                    AdjacencyList[j].push_back(i);
                    routeList[i][j].push_back(r);
                    routeList[j][i].push_back(r);

                }
            }
            for (int i = 0; i < VERTICES_NO; i++)
            {

                for (int j = i + 1; j < VERTICES_NO; j++)
                {
                    int minCost = INFINITY;
                    int minTransfer = INFINITY;
                    queue <node> Q;
                    node x = node(i, 0, 0, -1, vector <int> (VERTICES_NO, 0));
                    x.visitedNodes[x.vertex] = 1;
                    Q.push(x);
                    while (!Q.empty())
                    {
                        node currNode = Q.front();
                        Q.pop();
                        if (currNode.vertex == j)
                        {
                            if (currNode.totalCost < minCost)
                            {
                                minCost = currNode.totalCost;
                                minTransfer = currNode.totalTransfer;
                            }
                            continue;
                        }

                        for (list<int>::iterator it = AdjacencyList[currNode.vertex].begin(); it != AdjacencyList[currNode.vertex].end(); it++)
                        {
                            if (currNode.visitedNodes[*it] == 1)
                            {
                                continue;
                            }
                            for (list<int>::iterator r = routeList[currNode.vertex][*it].begin(); r != routeList[currNode.vertex][*it].end(); r++)
                            {
                                node neighbour(*it, currNode.totalCost + tr[currNode.vertex][*it], currNode.totalTransfer, *r, currNode.visitedNodes);
                                neighbour.visitedNodes[*it] = 1;
                                if (currNode.lastRoute != -1 && currNode.lastRoute != neighbour.lastRoute) //source node
                                {
                                    neighbour.totalTransfer++;
                                    neighbour.totalCost += TRANSFER_PENALTY;
                                }
                                if (neighbour.totalCost > minCost)
                                {
                                    continue;
                                }
                                Q.push(neighbour);

                            }
                        }

                    }
                    dist[i][j] = dist[j][i] = minCost;
                    transfer[i][j] = transfer[j][i] = minTransfer;
                }
            }

            double D[] = {0, 0, 0}, Dun = 0, ATT = 0, totalDemand = 0, totalSatisfied = 0,T = 0;

            for (int i = 0; i < VERTICES_NO; i++)
            {
                for (int j = i + 1; j < VERTICES_NO; j++)
                {
                    totalDemand += d[i][j];
                    if ( dist[i][j] == INFINITY)
                    {
                        Dun += d[i][j];
                    }
                    else if(transfer[i][j] > 2 )
                    {
                        Dun += d[i][j];
                        ATT += d[i][j] * dist[i][j];
                        totalSatisfied += d[i][j];
                      //  cout << "more than 2";
                    }
                    else
                    {
                        D[transfer[i][j]] += d[i][j];
                        ATT += d[i][j] * dist[i][j];
                        T += d[i][j] * dist[i][j];
                        totalSatisfied += d[i][j];
                    }
                }
            }

            T += Dun*(parameters["OptimalATT"] + 50);
            _eo.fitness(-T); //minimize
            _eo.D[0] = D[0]/totalDemand;
            _eo.D[1] = D[1]/totalDemand;
            _eo.D[2] = D[2]/totalDemand;
            _eo.Dun = Dun/totalDemand;
            _eo.ATT = ATT/totalSatisfied;
        }
    }

private:
    // START Private data of an RouteSetEvalFunc object
    //  varType anyVariable;		   // for example ...
    // END   Private data of an RouteSetEvalFunc object
};

#endif	/* _ROUTESETEVALFUNC_H */

