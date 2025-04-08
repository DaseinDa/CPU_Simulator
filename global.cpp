#include "global.h"
#include "historySnapshot.h"

namespace Global {
    int current_cycle=0;
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
    //Issue
    //ReservationStation
    deque<ReservationStationEntry> RS_INT_Queue;
    deque<ReservationStationEntry> RS_LOAD_Queue;
    deque<ReservationStationEntry> RS_STORE_Queue;
    deque<ReservationStationEntry> RS_FPadd_Queue;
    deque<ReservationStationEntry> RS_FPmult_Queue;
    deque<ReservationStationEntry> RS_FPdiv_Queue;
    deque<ReservationStationEntry> RS_FPBU_Queue;
    int rsFullNumber=0;
    int stallCount_RSFull=0;
    //ROB
    int stallCount_ROBFull=0;
    int robHead=0;
    int robTail=0;
    vector<ROBEntry> ROBuffer(NR);
    //unit pipeline
    deque<PipelineStage> INT_pipeline;
    deque<PipelineStage> LOAD_pipeline;
    deque<PipelineStage> STORE_pipeline;
    deque<PipelineStage> FPadd_pipeline;
    deque<PipelineStage> FPmult_pipeline;
    deque<PipelineStage> FPdiv_pipeline;
    deque<PipelineStage> FPBU_pipeline;
    //完成计算的指令
    vector<ReservationStationEntry> completeRSQueue;
}
unsigned int NF=4;
unsigned int NI=16;
unsigned int NW=4;
unsigned int NR=16;
bool DebugMode=true;

void printRenamingWorkerMapping(){
    cout<<"Renaming Worker Mapping:"<<endl;
    for(auto& mapping:Global::renaming_worker.registerRenamingMapping){
        cout<<mapping.first<<"->"<<mapping.second<<endl;
    }
}