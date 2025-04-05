#pragma once
#ifndef UNIT_H
#define UNIT_H

#include "global.h"
#include "instruction.h"
#include "btb.h"
#define IntLatency 1
#define LoadStoreLatency 1
#define FPaddLatency 3
#define FPmulLatency 4
#define FPdivLatency 6
#define BULatency 1

enum UnitType{
    INT,
    LoadStore,
    FPadd,
    FPmul,
    FPdiv,
    BU
};
class Unit{
    public:
        Unit();
        ~Unit();
        UnitType getUnitType(InstructionType instr_type);
        int getLatency(InstructionType instr_type);
};
#endif