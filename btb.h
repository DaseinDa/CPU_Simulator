#pragma once
#include <list>
#include <unordered_map>
#include <stdexcept> // 包含标准异常类
#include <iostream>
#include "branchPrediction.h"
// #include "global.h"
#include "instruction.h"
using namespace std;
class BTB{
    private:
        // const size_t capacity=MAX_BTB_SIZE;
        bool BTB_HIT[MAX_BTB_SIZE]={1};
    public:
       unordered_map<int,tuple<int, int, BranchPredict>> btbMap;//索引，TAG(instructionNumber), target位置，分支预测器状态
       BTB();
        ~BTB();
        int getTargetPosition(int instructionNumber);
        void update(int instructionNumber,bool taken);
        bool getPrediction(int instructionNumber);
        // void ifPredictTrueFalse(Instruction bne_instr);//根据唯一标识的bne指令恢复快照
};
