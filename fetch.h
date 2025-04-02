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
#include "btb.h"
using namespace std;
class Fetch{
    private:
        deque<Instruction> & fetchInstructionQueue;
        deque<Instruction>& instructionset;
        BTB& btb;
        const int nf;
        const unordered_map<string, size_t> labelMap;
        int fetch_pointer=0;
        //fetch to condition out-order branch prediction
    public:
        bool dispatch();
    Fetch(const Fetch&) = delete;
    Fetch& operator=(const Fetch&) = delete;
    Fetch(Fetch&&) = delete;             // 禁用移动构造
    Fetch& operator=(Fetch&&) = delete;  // 禁用移动赋值
    Fetch(deque<Instruction> & instructionset, deque<Instruction> & fetchInstructionQueue, BTB& btb, const int nf, const unordered_map<string, size_t>  labelMap);
};