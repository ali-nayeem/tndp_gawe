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

void floydWarshall(vector< vector<int> > & sDist, int vertexNo, vector< vector<int> > & transfer)
{
    for (int k = 0; k < vertexNo; k++)
    {
        for (int i = 0; i < vertexNo; i++)
        {
            for (int j = i + 1; j < vertexNo; j++)
            {
                if (sDist[i][k] == INFINITY || sDist[k][j] == INFINITY)
                {
                    continue;
                }
                if ((sDist[i][k] + sDist[k][j]) < sDist[i][j])
                {
                    sDist[j][i] = sDist[i][j] = sDist[i][k] + sDist[k][j];
                    transfer[i][j] = transfer[j][i] = transfer[i][k] + transfer[k][j];
                }
            }
        }
    }
}

template <class EOT>
class RouteSetEvalFunc : public eoEvalFunc<EOT>
{
public:
    /// Ctor - no requirement
    // START eventually add or modify the anyVariable argument

    RouteSetEvalFunc()
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
            vector< Route<double> > & routeSet = _eo.mutableRs();

            int dist[VERTICES_NO][VERTICES_NO];
            int transfer[VERTICES_NO][VERTICES_NO];
            list<int> O2E[VERTICES_NO];
            vector<int> E2O;
            vector<int> routeNo;
            vector< vector<int> > newMap(VERTICES_NO, vector<int> (routeSet.size(), 0));

            for (int r = 0; r < routeSet.size(); r++)
            {
                list<int>::const_iterator it = routeSet[r].R().begin();
                for (; it != routeSet[r].R().end(); it++)
                {
                    O2E[*it].push_back(E2O.size());
                    newMap[*it][r] = E2O.size();
                    E2O.push_back(*it);
                    routeNo.push_back(r);
                }
            }
            const int newVertexCount = E2O.size();
            vector< vector<int> > eDist(newVertexCount, vector<int>(newVertexCount, 0));
            vector< vector<int> > eTransfer(newVertexCount, vector<int>(newVertexCount, 0));
            for (int i = 0; i < newVertexCount; i++)
            {
                eDist[i][i] = 0;
                for (int j = i + 1; j < newVertexCount; j++)
                {
                    int I = E2O[i], J = E2O[j];
                    if (I == J)
                    {
                        eDist[i][j] = eDist[j][i] = TRANSFER_PENALTY;
                        eTransfer[i][j] = 1;
                    }
                    else
                    {
                        eDist[i][j] = eDist[j][i] = INFINITY;
                    }
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
                    int I = newMap[i][r];
                    int J = newMap[j][r];
                    eDist[J][I] = eDist[I][J] = tr[i][j];

                }
            }
            floydWarshall(eDist, newVertexCount, eTransfer);

            for (int i = 0; i < VERTICES_NO; i++)
            {
                dist[i][i] = 0;
                for (int j = i + 1; j < VERTICES_NO; j++)
                {
                    int min = INFINITY, minTransfer = INFINITY;

                    list<int>::iterator iIt = O2E[i].begin();
                    for (; iIt != O2E[i].end(); iIt++)
                    {
                        list<int>::iterator jIt = O2E[j].begin();
                        for (; jIt != O2E[j].end(); jIt++)
                        {
                            if (eDist[*iIt][*jIt] < min)
                            {
                                min = eDist[*iIt][*jIt];
                                minTransfer = eTransfer[*iIt][*jIt];
                            }
                        }
                    }

                    dist[i][j] = dist[j][i] = min;
                    transfer[i][j] = transfer[j][i] = minTransfer;
                }
            }

            double D[] = {0, 0, 0}, Dun = 0, ATT = 0, totalDemand = 0, totalSatisfied = 0, T = 0;

            for (int i = 0; i < VERTICES_NO; i++)
            {
                for (int j = i + 1; j < VERTICES_NO; j++)
                {
                    totalDemand += d[i][j];
                    if (dist[i][j] == INFINITY)
                    {
                        Dun += d[i][j];
                    }
                    else if (transfer[i][j] > 2)
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

            T += Dun * (parameters["OptimalATT"] + 50);
            _eo.fitness(-T); //minimize
            _eo.D[0] = D[0] / totalDemand;
            _eo.D[1] = D[1] / totalDemand;
            _eo.D[2] = D[2] / totalDemand;
            _eo.Dun = Dun / totalDemand;
            _eo.ATT = ATT / totalSatisfied;

        }
    }

private:
    // START Private data of an RouteSetEvalFunc object
    //  varType anyVariable;		   // for example ...
    // END   Private data of an RouteSetEvalFunc object
};

#endif	/* _ROUTESETEVALFUNC_H */

