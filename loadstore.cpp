#include "loadstore.h"
#include "global.h"
#include <algorithm>

bool LoadStore::existsFSDInROB(int target_rob_id) {
    int i = Global::robHead;
    while (i != target_rob_id) {
        const auto& entry = Global::ROBuffer[i];
        if (entry.opcode == fsd) {
            return true;
        }
        i = (i + 1) % NR;
    }
    return false;
}

void LoadStore::insertLoadQueue(const LoadResult& entry) {
    auto it = std::lower_bound(
        Global::LoadQueue.begin(),
        Global::LoadQueue.end(),
        entry,
        [](const LoadResult& a, const LoadResult& b) {
            return a.ID_in_Queue < b.ID_in_Queue;  // 升序排序标准
        }
    );
    Global::LoadQueue.insert(it, entry);  // 插入到正确位置
}
void LoadStore::insertPendingLoadQueue(const PendingLoad& entry) {
    auto it = std::lower_bound(
        Global::LoadHazardQueue.begin(),
        Global::LoadHazardQueue.end(),
        entry,
        [](const PendingLoad& a, const PendingLoad& b) {
            return a.ID_in_Queue < b.ID_in_Queue;  // 升序排序标准
        }
    );
    Global::LoadHazardQueue.insert(it, entry);  // 插入到正确位置
}
void LoadStore::insertStoreQueue(const ReadyStore& entry) {
    auto it = std::lower_bound(
        Global::StoreQueue.begin(),
        Global::StoreQueue.end(),
        entry,
        [](const ReadyStore& a, const ReadyStore& b) {
            return a.ID_in_Queue < b.ID_in_Queue;  // 升序排序标准
        }
    );
    Global::StoreQueue.insert(it, entry);  // 插入到正确位置
}

bool LoadStore::isIdNotInStoreVector(int ID_in_Queue) {
    vector<ReadyStore> vec=Global::StoreQueue;
    return std::find_if(vec.begin(), vec.end(), 
        [ID_in_Queue](const ReadyStore& elem) { 
            return elem.ID_in_Queue == ID_in_Queue; 
        }) == vec.end(); // 如果没找到，返回 true
}

bool LoadStore::allFSDReadyAndForwardValue(int fld_rob_id) {
    bool all_ready = true;
    int i = Global::robHead;

    while (i != fld_rob_id) {
        const auto& entry = Global::ROBuffer[i];

        if (entry.opcode == fsd) {
            if (isIdNotInStoreVector(entry.ID_in_Queue)) {
                all_ready = false;
                break;
            }
        }

        i = (i + 1) % NR;
    }

    return all_ready;
}

//StoreQueue升序排列，找出ID_Queue小于fld_rob_id且address一样的最晚的store
optional<double> LoadStore::findLatestStoreBeforeWithAddress(int address, int max_rob_id) {
    for (auto it = Global::StoreQueue.begin(); it != Global::StoreQueue.end(); ++it) {
        if (isInROBRange(Global::robHead, it->rob_id, max_rob_id, NR) &&
            it->address == address) {
            return it->value;
        }
    }
    return std::nullopt;
}

bool LoadStore::isInROBRange(int head, int target, int end, int ROB_SIZE) {
    if (head <= end)
        return head <= target && target < end;
    else
        return target >= head || target < end;
}
