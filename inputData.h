/*
 * File:   inputData.h
 * Author: MAN
 *
 * Created on September 5, 2013, 8:58 PM
 */

#ifndef _INPUTDATA_H
#define	_INPUTDATA_H

#include <climits>
//#include <map>
//#include <cstdlib>
//#include <list>

#define DEMAND_FILE "MandlOriginDestination.txt"  //change
#define TRAVEL_TIME_FILE "MandlDistances.txt"     //change
#define INITIAL_ROUTE_SET "inputPathMandl.txt"    //change
#define VERTICES_NO 15                            //change
#undef INFINITY
#define INFINITY INT_MAX
#define TRANSFER_PENALTY 5

using namespace std;

int d[VERTICES_NO][VERTICES_NO];
int tr[VERTICES_NO][VERTICES_NO];
int sDist[VERTICES_NO][VERTICES_NO];
int DS[VERTICES_NO][VERTICES_NO];
list <int> sPath[VERTICES_NO][VERTICES_NO];
vector <int> AdjList[VERTICES_NO];
map<string, float> parameters;

void readParam()
{
    ifstream fin("param");
    string name;
    float value;
    while (!fin.eof())
    {
        fin >> name >> value;
        cout << name << ":" << value << ",";
        parameters[name] = value;
        //        cout<<parameters[name]<<endl;
    }
    fin.close();

}

void readData()
{
    FILE * file = fopen(DEMAND_FILE, "r");
    FILE * file1 = fopen(TRAVEL_TIME_FILE, "r");
    for (int i = 0; i < VERTICES_NO; i++)
    {
        tr[i][i] = 0;
        for (int j = i + 1; j < VERTICES_NO; j++)
        {
            tr[j][i] = tr[i][j] = INFINITY;
        }
    }

    for (int i = 0; i < VERTICES_NO; i++)
    {
        for (int j = 0; j < VERTICES_NO; j++)
        {
            fscanf(file, "%d", &d[i][j]);
            fscanf(file1, "%d", &tr[i][j]);
            if (tr[i][j] != 0 && tr[i][j] != INFINITY)
            {
                AdjList[i].push_back(j);
            }
        }
    }
    fclose(file);
    fclose(file1);
}

list <int> findPath(int next[][VERTICES_NO], int i, int j) //called by floydWarshallWithPath()
{
    list <int> path;
    if (sPath[i][j].size() != 0)
    {
        return sPath[i][j];
    }
    if (sDist[i][j] == INFINITY)
    {
        cout << "No path between " << i << " and " << j;
        exit(0);
    }
    if (tr[i][j] != INFINITY)
    {
        path.push_back(i);
        path.push_back(j);
        return path;
    }
    else
    {
        int k = next[i][j];
        path = findPath(next, i, k);
        list <int> kj = findPath(next, k, j);
        path.pop_back();
        path.insert(path.end(), kj.begin(), kj.end());
        return path;
    }

}

void floydWarshallWithPath()
{

    int next[VERTICES_NO][VERTICES_NO];
    for (int i = 0; i < VERTICES_NO; i++)
    {
        sDist[i][i] = 0;
        for (int j = i + 1; j < VERTICES_NO; j++)
        {
            sDist[j][i] = sDist[i][j] = tr[i][j];
        }
    }

    for (int k = 0; k < VERTICES_NO; k++)
    {
        for (int i = 0; i < VERTICES_NO; i++)
        {
            for (int j = i + 1; j < VERTICES_NO; j++)
            {
                if (sDist[i][k] == INFINITY || sDist[k][j] == INFINITY)
                {
                    continue;
                }
                if ((sDist[i][k] + sDist[k][j]) < sDist[i][j])
                {
                    sDist[j][i] = sDist[i][j] = sDist[i][k] + sDist[k][j];
                    next[j][i] = next[i][j] = k;
                }
            }
        }
    }

    for (int i = 0; i < VERTICES_NO; i++)
    {
        for (int j = i + 1; j < VERTICES_NO; j++)
        {
            sPath[j][i] = sPath[i][j] = findPath(next, i, j);
            sPath[j][i].reverse();
        }
    }
    float ATT = 0, totalDemand = 0;
    for (int i = 0; i < VERTICES_NO; i++)
    {
        for (int j = i + 1; j < VERTICES_NO; j++)
        {
            totalDemand += d[i][j];
            ATT += d[i][j] * sDist[i][j];
        }
    }
    parameters["OptimalATT"] = ATT / totalDemand;
    cout << " OpATT" << parameters["OptimalATT"] << endl;
}

void findDS()
{
    for (int i = 0; i < VERTICES_NO; i++)
    {
        DS[i][i] = 0;
        for (int j = i + 1; j < VERTICES_NO; j++)
        {
            int sum = 0;
            list<int>::iterator outer = sPath[i][j].end();
            for (outer--; outer != sPath[i][j].begin(); outer--)
            {
                list<int>::iterator inner;
                for (inner = sPath[i][j].begin(); inner != outer; inner++)
                {
                    sum += d[*inner][*outer];
                }
            }
            DS[i][j] = DS[j][i] = sum;
        }
    }
}

void gatherAllInfo()
{
    readParam();
    readData();
    floydWarshallWithPath();
    findDS();
}

struct cellOfDS
{

    cellOfDS(int i, int j, int _ds)
    {
        a[0] = i;
        a[1] = j;
        ds = _ds;
    }
    int a[2];
    int ds;
};
//vector <cellOfDS> makeVectorOfDS()
//{
//    vector <cellOfDS>  temp;
//}
vector <cellOfDS> listCellofDS;

void makeVectorOfDS()
{
    for (int i = 0; i < VERTICES_NO; i++)
    {
        for (int j = i + 1; j < VERTICES_NO; j++)
        {
            listCellofDS.push_back(cellOfDS(i, j, DS[i][j]));
        }
    }
}

bool comparator(cellOfDS i, cellOfDS j)
{
    return (i.ds > j.ds);
}

void createInitialSolution()
{

    makeVectorOfDS();
    cout << "D-Matrix:\n";
    for (int i = 0; i < VERTICES_NO; i++)
    {
        for (int j = i + 1; j < VERTICES_NO; j++)
        {
            cout << d[i][j] << "\t";
        }
        cout << endl;
    }
    cout << "DS-Matrix:\n";
    for (int i = 0; i < VERTICES_NO; i++)
    {
        for (int j = i + 1; j < VERTICES_NO; j++)
        {
            cout << DS[i][j] << "\t";
        }
        cout << endl;
    }

    ofstream fout("inputPathFan.txt");
    for (int i = 0; i < parameters["routeSetSize"]; i++)
    {
        sort(listCellofDS.begin(), listCellofDS.end(), comparator);
        cout << "Path=";
        cout << listCellofDS[0].a[0] << "," << listCellofDS[0].a[1] << "," << listCellofDS[0].ds << endl;

        list<int>::iterator outer = sPath[listCellofDS[0].a[0]][listCellofDS[0].a[1]].end();
        for (outer--; outer != sPath[listCellofDS[0].a[0]][listCellofDS[0].a[1]].begin(); outer--)
        {

            //             int a = *outer;
            //             outer--;
            //             int b = *outer;
            //   d[a][b] = d[b][a] = 0;

            //cout<<"("<<a<<","<<b<<")"<<endl;
            //listCellofDS[i].ds = listCellofDS[i].ds - sDist[a][b];// i could not understand what is d[][] array contains.
            list<int>::iterator inner; // i think this is solvable in this way.
            for (inner = sPath[listCellofDS[0].a[0]][listCellofDS[0].a[1]].begin(); inner != outer; inner++)
            {
                d[*inner][*outer] = 0;
            }
            cout << *outer << "-";

        }
        cout << *outer << endl;
        for (list<int>::iterator inner = sPath[listCellofDS[0].a[0]][listCellofDS[0].a[1]].begin(); inner != sPath[listCellofDS[0].a[0]][listCellofDS[0].a[1]].end(); inner++)
        {
            fout << *inner << " ";
        }
        fout << "-1" << endl;
        findDS();
        //        cout<<"D-Matrix:\n";
        //        for (int i = 0; i < VERTICES_NO; i++)
        //        {
        //            for (int j = i + 1; j < VERTICES_NO; j++)
        //            {
        //                cout<<d[i][j]<< "\t";
        //            }
        //            cout<<endl;
        //        }
        //        cout<<"DS-Matrix:\n";
        //        for (int i = 0; i < VERTICES_NO; i++)
        //        {
        //            for (int j = i + 1; j < VERTICES_NO; j++)
        //            {
        //                cout<<DS[i][j]<< "\t";
        //            }
        //            cout<<endl;
        //        }
        listCellofDS.clear();
        makeVectorOfDS();

    }
    fout.close();
}

#endif	/* _INPUTDATA_H */

