#pragma once
#include <iostream>
#include <fstream>
#include <string.h>
#include <unistd.h>
#include <regex>
#include <vector>
#include <optional>
#include <sstream>
#include <unordered_map>
#include "global.h"
#include "common.h"
using namespace std;

class RegisterRenaming{
    private:
        // register renaming
        // renaming 只能按每行指令进行，不能直接抛进所有的指令，所以这里只实现了对单个指令的重命名
        unordered_map<string, string> registerRenamingMapping;//寄存器名最新对应的物理寄存器比如$0对应p0
        deque<string> RegisterRenamingFreeList;//可用的物理寄存器
        unordered_map<string, double> physicalRegister;
        // //history只有在bne才用得到
        // deque<unordered_map<string, string>> mappingTableHistory;//记录历史的寄存器重命名关系
        // deque<deque<string>> freeListHistory;
    public:
    ~RegisterRenaming(){initPhysicalRegs();};
    void initPhysicalRegs();
    bool instructionRegisterRenaming(Instruction& intr);
    void executeRegisterRenaming();
    bool isPhysicalRegsAvailable();
    string allocatePhysicalReg(string originRegName, bool isDesReg);
    string getPhysicalRegID(string originRegName);//根据寄存器名字得到对应的物理寄存器号
};
