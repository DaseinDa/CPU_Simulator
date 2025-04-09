#pragma once
#include "instruction_type.h"
#include <string>
#include <optional>
#include "common.h"
#include "loadstoretype.h"
using namespace std;

class LoadStore{    
    public:
    static void insertLoadQueue(const LoadResult& entry);
    static void insertStoreQueue(const ReadyStore& entry);
    static bool existsFSDInROB(int target_rob_id);
    static optional<double> findLatestStoreBeforeWithAddress(int address, int max_rob_id);
    static bool allFSDReadyAndForwardValue(int fld_rob_id);
    static bool isIdNotInStoreVector(int ID_in_Queue);
    static void insertPendingLoadQueue(const PendingLoad& entry);
    static bool isInROBRange(int head, int target, int end, int ROB_SIZE);
};
