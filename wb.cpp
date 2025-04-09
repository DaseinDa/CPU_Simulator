#include "wb.h"
//CDB的写回策略是先写回fsd指令，
//因为都是升序序，所以从front取就可以保证取最早的
//ALU最后写，因为都已经forwarding了
void WriteBack::writeBack(){
    for(int i=0;i<nb;i++){
        if(!Global::BUQueue.empty()){
            ReservationStationEntry entry=Global::BUQueue.front();
            Global::BUQueue.erase(Global::BUQueue.begin());
            Global::ROBuffer[entry.destROB].done=true;
            Global::ROBuffer[entry.destROB].value=entry.result;
            Global::ROBuffer[entry.destROB].predictTrueFalse=entry.predictTrueFalse.value();
            //如果有值说明mispredict了，正确预测没有值
            Global::instructionQueue[entry.ID_in_Queue].status_in_Queue=InstructionStatus::WB;
            continue;
        }
        if(!Global::StoreQueue.empty()){
            ReadyStore readyStore=Global::StoreQueue.front();
            Global::StoreQueue.erase(Global::StoreQueue.begin());
            Global::ROBuffer[readyStore.rob_id].done=true;
            Global::ROBuffer[readyStore.rob_id].memory_address=readyStore.address;
            Global::ROBuffer[readyStore.rob_id].value=readyStore.value;
            Global::instructionQueue[readyStore.ID_in_Queue].status_in_Queue=InstructionStatus::WB;
            continue;
        }

        if(!Global::LoadQueue.empty()){
            LoadResult loadResult=Global::LoadQueue.front();
            Global::LoadQueue.erase(Global::LoadQueue.begin());
            Global::ROBuffer[loadResult.rob_id].done=true;
            Global::instructionQueue[loadResult.ID_in_Queue].status_in_Queue=InstructionStatus::WB;
            continue;
        }

        if(!Global::completeRSQueue.empty()){
            ReservationStationEntry entry=Global::completeRSQueue.front();
            Global::completeRSQueue.erase(Global::completeRSQueue.begin());
            Global::instructionQueue[entry.ID_in_Queue].status_in_Queue=InstructionStatus::WB;
            continue;
        }

    }
    //检查PendingLoadQueue里有没有fsd可以forwarding的
    for(auto it = Global::LoadHazardQueue.begin(); it != Global::LoadHazardQueue.end(); ) {
        int already = LoadStore::allFSDReadyAndForwardValue(it->rob_id);
        if(already) {
            optional<double> value = LoadStore::findLatestStoreBeforeWithAddress(it->address, it->rob_id);
            if(value.has_value()) {
                writePRF(it->prf_id, value.value()); // 直接 forwarding
            } else {
                double value = Global::memory_value[it->address];
                writePRF(it->prf_id, value); // 直接 forwarding
            }
            //加入LoadQueue
            LoadResult loadResult;
            loadResult.rob_id=it->rob_id;
            loadResult.prf_id=it->prf_id;
            loadResult.value=value.value();
            LoadStore::insertLoadQueue(loadResult);
            it = Global::LoadHazardQueue.erase(it); // 删除当前元素并返回下一个有效迭代器
        } else {
            ++it; // 如果未处理则继续下一个
        }
    }

    
}