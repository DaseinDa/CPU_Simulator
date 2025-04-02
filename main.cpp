#include <iostream>
#include <string.h>
#include<unistd.h>
// #include "global.h"
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
    /*loop processing parameters*/
    while(EOF != (c = getopt(argc,argv,"han:")))
    {
        //Print the command parameters
        printf("start to process %d para\n",optind);
        switch(c)
        {
            case 'p':
                printf("we get option -f\n");
                PathFile=optarg;
                break;
            case 'f':
                printf("we get option -a\n");
                break;
            //-n选项必须要参数
            case 'i':
                printf("we get option -ni,para is %s\n",optarg);
                break;
            case 'w':
                printf("we get option -nw,para is %s\n",optarg);
                break;
            case 'b':
                printf("we get option -nb,para is %s\n",optarg);
                break;
            case 'r':
                printf("we get option -nr,para is %s\n",optarg);
                break;
            //表示选项不支持
            case '?':
                printf("unknow option:%c\n",optopt);
                break;
            default:
                break;
        }    
    }
    Simulator simulator;
    simulator.ReadAssemblyFile("assembly.dat");
    simulator.pipelineGlobalCycle();
    return 0;
}