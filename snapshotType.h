#pragma once
#include "instruction.h"
#include "renaming.h"
#include "btb.h"
#include <deque>
#include <unordered_map>
#include <vector>
#include "ROB.h"
#include "unit_pip.h"
#include "loadstore.h"
#include "loadstoretype.h"
using namespace std;
struct Snapshot {
    ~Snapshot();
    Instruction bne_instruction;
    int current_cycle=0;
    unordered_map<string, size_t> labelMap;
    unordered_map<int, double> memory_value;
    RegisterRenaming renaming_worker;
    deque<Instruction> instructionQueue;
    deque<Instruction> fetchInstructionQueue;   
    int fetch_pointer=0;
    BTB btb;
    unordered_map<string, ArchitectureRegister> architectureRegisterFile;
    deque<Instruction> decodeInstructionQueue;
    int renameStall=0;
    //Issue
    //ReservationStation
    deque<ReservationStationEntry> RS_INT_Queue;
    deque<ReservationStationEntry> RS_LOAD_Queue;
    deque<ReservationStationEntry> RS_STORE_Queue;
    deque<ReservationStationEntry> RS_FPadd_Queue;
    deque<ReservationStationEntry> RS_FPmult_Queue;
    deque<ReservationStationEntry> RS_FPdiv_Queue;
    deque<ReservationStationEntry> RS_BU_Queue;
    int rsFullNumber=0;
    int stallCount_RSFull=0;
    //ROB
    int stallCount_ROBFull=0;
    int robHead=0;
    int robTail=0;
    vector<ROBEntry> ROBuffer;
    //unit pipeline
    deque<PipelineStage> INT_pipeline;
    deque<PipelineStage> LOAD_pipeline;
    deque<PipelineStage> STORE_pipeline;
    deque<PipelineStage> FPadd_pipeline;
    deque<PipelineStage> FPmult_pipeline;
    deque<PipelineStage> FPdiv_pipeline;
    deque<PipelineStage> BU_pipeline;
    //完成计算的指令
    vector<ReservationStationEntry> completeRSQueue;
    vector<ReservationStationEntry> BUQueue;
    //load store
    vector<LoadResult> LoadQueue;
    vector<PendingLoad> LoadHazardQueue;
    vector<ReadyStore> StoreQueue;
};
