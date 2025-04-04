#pragma once
#include "string.h"
#include <iostream>
#include <vector>
#include <unordered_map>
#include <iomanip>
#include <deque>
#include <optional>
#include <sstream>
#include "BranchPredictStage.h"
#include "instruction.h"
#include "btb.h"
#include "renaming.h"
#include "snapshotType.h"

// #include "global.h"
using namespace std;

class HistorySnapshot {
    private:
        deque<Snapshot> history_snapshots;
    public:
        HistorySnapshot();
        Snapshot* findMatchingSnapshot(int instruction_number, int execution_count);
        void addSnapshot(Instruction bne_instruction);
        void predictionTrueFalseRecover(Instruction bne_instruction, bool actucalPrediction);
        void printHistory();
        void printSnapshot(const Snapshot snapshot);
};
