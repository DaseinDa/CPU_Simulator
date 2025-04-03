#include "BranchPredictStage.h"

int getBTBposition(int instructionNumber){
    int hexAddress = instructionNumber;  // Assume instructionNumber is already a byte address
    int bits7to4 = (hexAddress >> 4) & 0xF;  // Shift right by 4, then mask 4 bits
    return bits7to4 % MAX_BTB_SIZE;  // Ensure it's within 0-15 (redundant if MAX_BTB_SIZE=16)
}