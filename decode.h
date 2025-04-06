#pragma once
#include "global.h"
#include "instruction.h"
#include "btb.h"
#include "renaming.h"
class Decode{
    private:
        const int ni=NI;
    public:
        bool dispatch();
        Decode();
        ~Decode();
        void decode(Instruction& instruction);
};
