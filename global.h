#pragma once
#include "string.h"
#include <iostream>
#include <vector>
#include <unordered_map>
#include <iomanip>
#include <deque>
#include <optional>
#include <sstream>
#include "common.h"
#include "BranchPredictStage.h"
class BTB;//Forward declaration
#include "btb.h"
#include "instruction.h"
class RegisterRenaming;//Forward declaration
#include "renaming.h"
class HistorySnapshot;//Forward declaration
using namespace std;
#include "archiRegister.h"
#include "dependency.h"
#include "reservationStation.h"
#include "ROB.h"
#include "unit_pip.h"
extern unsigned int NF;
extern unsigned int NI;
extern unsigned int NW;
extern unsigned int NB;
extern unsigned int NR;
extern bool DebugMode;


namespace Global {
    extern int current_cycle;
    extern unordered_map<string, size_t> labelMap;
    extern unordered_map<int, double> memory_value;
    extern RegisterRenaming renaming_worker;
    extern vector<Instruction> instructions;
    extern deque<Instruction> instructionset;
    //记录程序顺序
    extern deque<Instruction> instructionQueue;//也需要能标识每条指令处于fetch-commit的哪个阶段
    extern deque<Instruction> fetchInstructionQueue;
    extern int fetch_pointer;
    extern BTB btb;
    extern HistorySnapshot historySnapshot;
    extern unordered_map<string, ArchitectureRegister> architectureRegisterFile;
    //decode
    extern deque<Instruction> decodeInstructionQueue;
    extern int renameStall;
    //依赖关系
    extern map<int,DependencyList> dependency_map;   

    //ReservationStation
    extern deque<ReservationStationEntry> RS_INT_Queue;
    extern deque<ReservationStationEntry> RS_LOAD_Queue;
    extern deque<ReservationStationEntry> RS_STORE_Queue;
    extern deque<ReservationStationEntry> RS_FPadd_Queue;
    extern deque<ReservationStationEntry> RS_FPmult_Queue;
    extern deque<ReservationStationEntry> RS_FPdiv_Queue;
    extern deque<ReservationStationEntry> RS_BU_Queue;


    extern int rsFullNumber;//保留站满导致指令不能发射的次数，注意这不是stall cycle数
    extern int stallCount_RSFull;
    //ROB
    extern int stallCount_ROBFull;
    extern int robHead;
    extern int robTail;
    extern vector<ROBEntry> ROBuffer;

    //unit pipeline
    extern deque<PipelineStage> INT_pipeline;
    extern deque<PipelineStage> LOAD_pipeline;
    extern deque<PipelineStage> STORE_pipeline;
    extern deque<PipelineStage> FPadd_pipeline;
    extern deque<PipelineStage> FPmult_pipeline;
    extern deque<PipelineStage> FPdiv_pipeline;
    extern deque<PipelineStage> BU_pipeline;
    //完成计算的指令,等待写CDB,ROB
    extern vector<ReservationStationEntry> completeRSQueue;
}

string getInstructionAddress(int instructionNumber);
int getBTBposition(int instructionNumber);



// 调转执行示例:
// else if (inst.opcode == "ne") {
//         // bne rs, rt, target
//         if (registers[inst.operands[0]] != registers[inst.operands[1]]) {
//             // 跳转到目标标签
//             pc = labelMap.at(inst.target.value());
//             return; // 跳过 pc++
//         }}

// labelMap[label] = index
// if (colonPos != std::string::npos) {
//     label = line.substr(0, colonPos);
//     line = line.substr(colonPos + 1);
//     line.erase(0, line.find_first_not_of(" \t"));
//     labelMap[label] = index; // 记录标签对应的指令索引
        // } 

//用一个数组来记录label_map,每个label对应指令中的第几行