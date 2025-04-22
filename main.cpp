#include <iostream>
#include <string.h>
#include <vector>
#include<unistd.h>
#include "global.h"
#include "simulator.h"
using namespace std;
extern int optind,opterr,optopt;
extern char *optarg;

//global NF, NW, NR, NB, NI
// ReadFile();
// Error_report();extern

int main(int argc, char *argv[])
{
    cout<< "Hello World" << endl;
    // NF--;
    int c = 0; //receive the option
    char* PathFile=optarg;
    PathFile = "assembly.dat";
    unsigned int NF=4;
    unsigned int NI=16;
    unsigned int NW=4;
    unsigned int NR=16;
    unsigned int NB=4;
    /*loop processing parameters*/
    while(EOF != (c = getopt(argc,argv,"han:")))
    {
        //Print the command parameters
        printf("start to process %d para\n",optind);
        switch(c)
        {
            case 'p':
                printf("we get option file -p\n");
                PathFile=optarg;
                break;
            case 'f':
                printf("we get option -f NFi, NI\n");
                NF=atoi(optarg);
                break;
            //-n option must have input parameters
            case 'i':
                printf("we get option -ni,para is %s\n",optarg);
                NI=atoi(optarg);
                break;
            case 'w':
                printf("we get option -nw,para is %s\n",optarg);
                NW=atoi(optarg);
                break;
            case 'b':
                printf("we get option -nb,para is %s\n",optarg);
                NB=atoi(optarg);
                break;
            case 'r':
                printf("we get option -nr,para is %s\n",optarg);
                NR=atoi(optarg);
                break;
            //not supporting options
            case '?':
                printf("unknow option:%c\n",optopt);
                break;
            default:
                break;
        }    
    }
    Global::ROBuffer = vector<ROBEntry>(NR);
    Simulator simulator;
    simulator.ReadAssemblyFile(PathFile);
    simulator.run();
    return 0;
}
