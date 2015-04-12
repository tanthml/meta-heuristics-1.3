/* 
 * File:   mtspMain.cpp
 * Author: tieuminh
 *
 * Created on April 1, 2015, 4:33 PM
 */


#include "./lib/mtspLibrary.h"

using namespace std;


int main(int argc, char** argv) {
 
    mtspProblem graph("hcm16.tsp");    
    mtspSolution route(graph);
    edaPopulation pop;
    edaSearch* search;
    pop.init(route, 1000);
    edaLoopCondition crit(1500);
    mtsp2Opt opt;
    edaGreedyStrategy* stgy = new edaBestImprovement();
    search = new edaHC(opt, *stgy, crit); 
    search->search(pop);

    mtspSolution& result = *(mtspSolution*) pop[pop.best()];
    cout << result;
    

    return 0;
}
