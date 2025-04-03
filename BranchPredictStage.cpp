#include "BranchPredictStage.h"

int getBTBposition(int instructionNumber){
    return instructionNumber%MAX_BTB_SIZE;
}