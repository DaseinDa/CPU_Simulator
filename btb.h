#pragma once
#include <list>
#include <unordered_map>
#include "branchPrediction.h"
#include "global.h"
using namespace std;
class BTB{
    private:
        list<string> btbList;//string is the 4-7 address of the bne instructio
        const size_t capacity=MAX_BTB_SIZE;
    public:
       unordered_map<string, BranchPredict> btbMap;//string is the 4-7 address of the bne instructio
        ~BTB();
        void update(int instructionNumber,bool taken);
};
