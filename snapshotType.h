#pragma once
#include "instruction.h"
#include "renaming.h"
#include "btb.h"
#include <deque>
#include <unordered_map>
struct Snapshot {
        ~Snapshot();
        //bne id
        Instruction bne_instruction;
        unordered_map<int, double> memory_value;
        //renaming
        RegisterRenaming renaming_worker;
        deque<Instruction> instructionset;//only change instructionset during the simulation
        deque<Instruction> fetchInstructionQueue;   
        int fetch_pointer=0;
        //BTB
        BTB btb;
        //decode
        //issue
        //cdb
        //reservation station
        //execute
        //rob
        //write back
        //commit
};
