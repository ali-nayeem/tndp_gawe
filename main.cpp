// standard includes
#include <iostream>

// the general include for eo
#include <eo>
#include "eoGAWE.h"

using namespace std;

#include "routeSet.h"
#include "inputData.h"
#include "popInit.h"
#include "routeFitness.h"
#include "routeSetEvalFunc.h"
#include "routeSetQuadCrossover.h"
#include "routeSetMutation.h"

typedef eoMinimizingFitness MyFitT;
typedef RouteSet<double> Indi;


ofstream fout("best.txt");
double avg = 0;

void main_function(int argc, char **argv)
{
    rng.reseed(time(0));

    //fitness evaluators
    eoEvalFuncPtr< Route<double> > routeEval(routeFitness);
    RouteSetEvalFunc<Indi> routeSetEval;

    //population
    eoPop<Indi> pop;
    popInit(pop, parameters["popSize"], parameters["routeSetSize"], routeSetEval);

    //selection
    eoDetTournamentSelect<Indi> selectOne(parameters["tFit"]);
    double perc = (parameters["popSize"] - parameters["eilte"]) / parameters["popSize"]; //eilte
    eoSelectPerc<Indi> select(selectOne, perc);

    //crossover
    RouteSetQuadCrossover<Indi> xover(parameters["pSwap"]);

    //mutations
    BigMutation<Indi> bM(routeEval);
    SmallMutation<Indi> sM(parameters["pDelete"], routeEval);
    eoPropCombinedMonOp<Indi> mutation(sM, parameters["pSm"]);
    mutation.add(bM, 1 - parameters["pSm"]);

    //stop after maxGen generations
    eoGenContinue<Indi> continuator(parameters["maxGen"]);
    //CHECKPOINT
    eoCheckPoint<Indi> checkpoint(continuator);
    // Create a counter parameter
    eoValueParam<unsigned> generationCounter(0, "Generation");
    // Create an incrementor (sub-class of eoUpdater). Note that the
    // parameter's value is passed by reference,
    // so every time the incrementer is updated (every generation),
    // the data in generationCounter will change.
    eoIncrementor<unsigned> increment(generationCounter.value());
    // Add it to the checkpoint,
    // so the counter is updated (here, incremented) every generation
    checkpoint.add(increment);

    // now some statistics on the population:
    // Best fitness in population
    eoBestFitnessStat<Indi> bestStat;
    // Second moment stats: average and stdev
    eoSecondMomentStats<Indi> SecondStat;

    // Add them to the checkpoint to get them called at the appropriate time
    checkpoint.add(bestStat);
    checkpoint.add(SecondStat);

    // The Stdout monitor will print parameters to the screen ...
    eoStdoutMonitor monitor(false);

    // when called by the checkpoint (i.e. at every generation)
    checkpoint.add(monitor);

    // the monitor will output a series of parameters: add them
    monitor.add(generationCounter);
    monitor.add(bestStat);
    monitor.add(SecondStat);

    // A file monitor: will print parameters to ... a File, yes, you got it!
    //eoFileMonitor fileMonitor("stats.txt", " ");

    // the checkpoint mechanism can handle multiple monitors
   // checkpoint.add(fileMonitor);

    // the fileMonitor can monitor parameters, too, but you must tell it!
   // fileMonitor.add(generationCounter);
  //  fileMonitor.add(bestStat);
  //  fileMonitor.add(SecondStat);

    //THE ALGORITHM
    eoGAWE<Indi> gga(select, xover, mutation,
                     routeSetEval, checkpoint, parameters["elite"]);

    gga.best = pop[0];
    cout << "Initial : " << gga.best.fitness() << endl << gga.best.D[0] << endl << gga.best.D[1] << endl << gga.best.D[2] << endl << gga.best.Dun << endl << gga.best.ATT<<endl;
    // Apply algo to pop - that's it!
    gga(pop);

    //output
    cout << "FINAL Population size\n" << pop.size() << endl;
    cout << "Best : " << gga.best << endl << gga.best.D[0] << endl << gga.best.D[1] << endl << gga.best.D[2] << endl << gga.best.Dun << endl << gga.best.ATT;
    
    fout << "Best : " << gga.best << endl << gga.best.D[0] << endl << gga.best.D[1] << endl << gga.best.D[2] << endl << gga.best.Dun << endl << gga.best.ATT <<endl;
    avg += gga.best.D[0];

    //  pop.sort();

}

int main(int argc, char** argv)
{
      gatherAllInfo();
//    createInitialSolution();
//    exit(0);

    for(int j = 0; j<parameters["run"] ; j++)
    {
        fout<<"Run:"<<(j+1)<<endl;
        try
        {
            main_function(argc, argv);
        }
        catch (exception& e)
        {
            cout << "Exception: " << e.what() << '\n';
        }

        fout<<"Current Avg D0:"<<avg/(j+1);
        fout<<"\n\n"<<endl;
    }
    fout<<"Avg D0:"<<avg/parameters["run"];
    fout.close();
    return (EXIT_SUCCESS);
}
