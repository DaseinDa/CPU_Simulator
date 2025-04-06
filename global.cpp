#include "global.h"
#include "historySnapshot.h"

namespace Global {
    unordered_map<string, size_t> labelMap;
    unordered_map<int, double> memory_value;
    RegisterRenaming renaming_worker;
    vector<Instruction> instructions;//the original instructions we can check during the simulation
    deque<Instruction> instructionset;//only change instructionset during the simulation
    deque<Instruction> instructionQueue;
    deque<Instruction> fetchInstructionQueue;   
    int fetch_pointer=0;
    BTB btb;
    HistorySnapshot historySnapshot;
    unordered_map<string, ArchitectureRegister> architectureRegisterFile;
    deque<Instruction> decodeInstructionQueue;
    int renameStall=0;
    map<int,DependencyList> dependency_map;  
}
unsigned int NF=4;
unsigned int NI=16;
bool DebugMode=true;

void printRenamingWorkerMapping(){
    cout<<"Renaming Worker Mapping:"<<endl;
    for(auto& mapping:Global::renaming_worker.registerRenamingMapping){
        cout<<mapping.first<<"->"<<mapping.second<<endl;
    }
}