#include "global.h"


namespace Global {
    unordered_map<string, size_t> labelMap;
    unordered_map<int, double> memory_value;
    RegisterRenaming renaming_worker;
    vector<Instruction> instructions;//the original instructions we can check during the simulation
    deque<Instruction> instructionset;//only change instructionset during the simulation
    deque<Instruction> fetchInstructionQueue;   
    int fetch_pointer=0;
    BTB btb;
    // HistorySnapshot historySnapshot;
}
unsigned int NF=4;
bool DebugMode=true;