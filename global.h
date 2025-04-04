#pragma once
#include "string.h"
#include <iostream>
#include <vector>
#include <unordered_map>
#include <iomanip>
#include <deque>
#include <optional>
#include <sstream>
#include "BranchPredictStage.h"
class BTB;//Forward declaration
#include "btb.h"
#include "instruction.h"
class RegisterRenaming;//Forward declaration
#include "renaming.h"
class HistorySnapshot;//Forward declaration
// #include "historySnapshot.h"
using namespace std;
#define RESGITER_NUMBER 32
#define ZERO_REGISTER "$0"
extern unsigned int NF;
extern unsigned int NI;
extern unsigned int NW;
extern unsigned int NB;
extern unsigned int NR;
extern bool DebugMode;


namespace Global {
    extern unordered_map<string, size_t> labelMap;
    extern unordered_map<int, double> memory_value;
    extern RegisterRenaming renaming_worker;
    extern vector<Instruction> instructions;
    extern deque<Instruction> instructionset;
    extern deque<Instruction> fetchInstructionQueue;
    extern int fetch_pointer;
    extern BTB btb;
    // extern HistorySnapshot historySnapshot;
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