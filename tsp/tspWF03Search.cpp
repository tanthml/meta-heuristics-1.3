/* 
 * File:   main.cpp
 * Author: Tieu Minh
 *
 * Created on April 7, 2014, 4:24 PM
 */

#include "./lib/tspLibrary.h"


#define N graph.size()

using namespace std;    


#if COMM_MPI
    static edaMpiWorkflow workflow;
#else
    static edaSeqWorkflow workflow;
#endif
    
int main(int argc, char** argv)
{    
    EDAMetasearchStart(argc, argv);
    if (argc != 2)
    {
      std::cerr << "Usage : ./" <<  __progname
                << " [instance]" << std::endl;
    }
    else
    {
        tspProblem graph(argv[1]); 
        tspSolution route(graph);   
        
        edaBestImprovement stgy;
        edaExpCoolingSchedule sched(0, N, 0.98);   
        edaBestAspirCrit aspir; 
        edaStaticTabu tabu(N);
        
        
        tsp2Opt opt;
        edaPopulation pop;
        pop.init(route, N);
        edaTimeCondition crit( N/10. );   
        edaLoopCondition loop(1);   
        edaEliteCollect elite(N);
        
        edaSA saSearch(opt, sched, crit);
        edaTS tsSearch(opt, aspir, tabu, crit);
        edaHC hcSearch(opt, stgy, crit);
        
        
        unsigned int tsid0 = workflow.insertVertex (tsSearch);
        unsigned int said1 = workflow.insertVertex (saSearch);   
        unsigned int said2 = workflow.insertVertex (saSearch);  
        unsigned int said3 = workflow.insertVertex (saSearch);  
        unsigned int hcid4 = workflow.insertVertex (hcSearch, elite);
       
        workflow.insertEdge(tsid0, said1);
        workflow.insertEdge(tsid0, said2);
        workflow.insertEdge(tsid0, said3);
        workflow.insertEdge(said1, hcid4);
        workflow.insertEdge(said2, hcid4);
        workflow.insertEdge(said3, hcid4);
        workflow.insertLoop(hcid4, tsid0, loop);
        
        workflow.set(graph);
        workflow.search(pop);
        
        tspSolution& result = *(tspSolution*) pop[pop.best()];
        cout << "[Route] " << result << endl;
        cout << "[Fitness] " << result.evaluate () << endl;  
        
    }
    EDAMetasearchStop();
    return 0;
}
edaSerialize* userClassGenerate (unsigned int clsid)
{
    switch (clsid)
    {
        case _USERCLASSID_ + _CLSID_TSPPROBLEM_:
            return new tspProblem();
            
        case _USERCLASSID_ + _CLSID_TSPSOLUTION_:
            return new tspSolution();
        
        case _USERCLASSID_ + _CLSID_TSP2OPT_:
            return new tsp2Opt();    

        case _USERCLASSID_ + _CLSID_TSPINSERTION_:
            return new tspInsertion();    
            
        default:
            cerr << "Unknown classId " << clsid << " for object generation !" << endl;
            exit (-1);
    }
}
