#include <iostream>
#include <fstream>
#include <string.h>
#include<unistd.h>
#include <regex>
#include <vector>
#include <optional>
#include <sstream>
#include <unordered_map>
#include "global.h"
#include "renaming.h"
#include "fetch.h"
#include "decode.h"
#include "issue.h"
#include "execute.h"
#include "btb.h"
#include "instruction.h"
using namespace std;

void ReadAssemblyFile(char * PathFile);

class Simulator{
    private:
        string PathFile = "";
        const int nf=4, ni=0, nw=0, nb=0, nr=0;
        int global_cycle=0;
        // unordered_map<int, double> memory;
        unsigned int index=0;
        //instruction set
        // vector<Instruction> instructions;
        // deque<Instruction> instructionset;
        //fetch instruction queue
        // deque<Instruction> fetchInstructionQueue;
        // // register renaming
        // unordered_map<string, double> physicalRegister;
        // deque<string> RegisterRenamingFreeList;
        // unordered_map<string, string> registerRenamingMapping;
        RegisterRenaming renaming_worker;
        //Stall
        int fetchStall=0;
        int decodeStall=0;
        int issueStall=0;
        //branch prediction with 16 entry
    //     BTB btb;
    public:
    Simulator();
    ~Simulator();
    void ReadAssemblyFile(char * PathFile);
    void readInstruction(string line);
    void readMemory(string line);
    void pipelineGlobalCycle();

    //Register renaming
    // void initSimulatorPhysicalRegs();

    //fetch
    Fetch *f;
    Decode *d;
    Issue *i;
    Execute *e;
    //execute
    void execute();
    void commitInstruction();


    //print
    void printLabelMap(){
        for (auto it = Global::labelMap.begin(); it != Global::labelMap.end(); ++it) {
            std::cout << "LabelMap " << it->first << ", Index: " << it->second << std::endl;
        }
    }
};