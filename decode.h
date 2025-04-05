#pragma once
#include "global.h"
#include "instruction.h"
#include "btb.h"

class Decode{
    private:
    public:
        Decode();
        ~Decode();
        void decode(Instruction& instruction);
};
