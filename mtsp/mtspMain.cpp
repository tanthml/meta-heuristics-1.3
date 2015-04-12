/* 
 * File:   mtspMain.cpp
 * Author: tieuminh
 *
 * Created on April 1, 2015, 4:33 PM
 */


#include "./lib/mtspLibrary.h"

using namespace std;


#if COMM_MPI
    static edaMpiWorkflow workflow;
#else
    static edaSeqWorkflow workflow;
#endif
    
int main(int argc, char** argv) {
    EDAMetasearchStart(argc, argv);
    if (argc < 6) {
        std::cerr << "Usage : ./" << __progname
                << " [Problem] [HC] [Best/First] [NumLoop] [NumPop]\n"
                << " Or : ./" << __progname
                << " [Problem] [SA] [Linear/Exponent] [NumLoop] [NumPop] [InitTemp] [Threshold] [Quantity/Ratio]\n"
                << " Or : ./" << __progname
                << " [Problem] [TS] [Best/No] [NumLoop] [NumPop] [NumTabuList]\n"
                << " Or : ./" << __progname
                << " [Problem] [GA] [Rank/Weight] [NumLoop] [NumPop] [SelectRate] [CrossRate] [MuteRate]\n";
    } else {
        mtspProblem graph(argv[1]);       
        mtspSolution route(graph);

        edaPopulation pop;
        edaSearch* search;
        pop.init(route, atof(argv[5]));
        edaLoopCondition crit(atof(argv[4]));


        if (strcmp(argv[2], "HC") == 0) {
            mtsp2Opt opt;
            edaGreedyStrategy* stgy;
            if (strcmp(argv[3], "Best") == 0) {
                stgy = new edaBestImprovement();
            } else if (strcmp(argv[3], "First") == 0) {
                stgy = new edaFirstImprovement();
            }
            search = new edaHC(opt, *stgy, crit);
        } else if (strcmp(argv[2], "SA") == 0) {
            mtsp2Opt opt;
            edaCoolingSchedule* sched;
            if (strcmp(argv[3], "Linear") == 0) {
                sched = new edaLinearCoolingSchedule(atof(argv[6]), atof(argv[8]), atof(argv[7]));
            } else if (strcmp(argv[3], "Exponent") == 0) {
                sched = new edaExpCoolingSchedule(atof(argv[6]), atof(argv[8]), atof(argv[7]));
            }
            search = new edaSA(opt, *sched, crit);
        } else if (strcmp(argv[2], "TS") == 0) {
            mtsp2Opt opt;
            edaAspirCrit* aspir;
            if (strcmp(argv[3], "Best") == 0) {
                aspir = new edaBestAspirCrit();
            } else if (strcmp(argv[3], "No") == 0) {
                aspir = new edaNoAspirCrit();
            }
            edaStaticTabu tabu(atoi(argv[8]));
            search = new edaTS(opt, *aspir, tabu, crit);

        } 
        else if (strcmp(argv[2], "GA") == 0) {
            edaNaturalSelection* nslect;   
            if (strcmp(argv[3], "Rank") == 0) {
                nslect = new edaRankSelection(atof(argv[6]));
            } else if (strcmp(argv[3], "Weight") == 0) {
                //Error
                nslect = new edaRankSelection(atof(argv[6]));
            }
            edaPMCrossover cross(atof(argv[7]));
            edaRandSwapMutation mute(atof(argv[8]));
            search = new edaGA(*nslect, cross, mute, crit); 
        }
        
        workflow.set(graph);
        workflow.insertVertex(*search);
        workflow.search(pop);
        
        mtspSolution& result = *(mtspSolution*) pop[pop.best()];
        cout << result.pro();
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
        case _USERCLASSID_ + _CLSID_MTSPPROBLEM_:
            return new mtspProblem();
            
        case _USERCLASSID_ + _CLSID_MTSPSOLUTION_:
            return new mtspSolution();
        
        case _USERCLASSID_ + _CLSID_MTSP2OPT_:
            return new mtsp2Opt();    
            
        default:
            cerr << "Unknown classId " << clsid << " for object generation !" << endl;
            exit (-1);
    }
}
