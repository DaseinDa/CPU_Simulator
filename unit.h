#pragma once
#ifndef UNIT_H
#define UNIT_H

#include "global.h"
#include "btb.h"
#include "instruction_type.h"
#include "unit_type.h"
#define IntLatency 1
#define LoadStoreLatency 1
#define FPaddLatency 3
#define FPmulLatency 4
#define FPdivLatency 6
#define BULatency 1

class Unit{
    private:
        UnitType unit_function;//代表这个Unit是哪个单元
        bool isAvailable=true;//Only for non-pipelined units
        int countLatency=0;//Only for non-pipelined units
    public:
        Unit();
        Unit(UnitType unit);
        ~Unit();
        static UnitType getUnitType(InstructionType instr_type);
        static int getInstrLatency(InstructionType instr_type);
        static int getUnitLatency(UnitType unit);
        void calculateInCycle();
        void busy();
        void free();
};
#endif