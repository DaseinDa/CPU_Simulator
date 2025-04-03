#pragma once
#define MAX_BTB_SIZE 16
enum BranchPredictionStage {
    PREDICT_WEAK_TAKEN,
    PREDICT_STRONG_TAKEN,
    PREDICT_WEAK_NOT_TAKEN,
    PREDICT_STRONG_NOT_TAKEN,
};
int getBTBposition(int instructionNumber);