#include "wb.h"
//CDB的写回策略是先写回fsd指令，
//因为都是升序序，所以从front取就可以保证取最早的
//ALU最后写，因为都已经forwarding了
WriteBack::WriteBack(){

}
void WriteBack::writeBack(){
    printCompleteQueue();
    cout<<"-------------------------writeBack start------------------------"<<endl;
    for(int i=0;i<nb;i++){
        if(!Global::completeRSQueue.empty()){
            cout<<"-------------------------completeQueue "<<Global::completeRSQueue.front().ID_in_Queue<<"------------------------"<<endl;
            ReservationStationEntry entry=Global::completeRSQueue.front();
            Global::completeRSQueue.erase(Global::completeRSQueue.begin());
            Global::ROBuffer[entry.destROB].done=true;
            Global::renaming_worker.physicalRegister[entry.destPhysicalRegister].isReady=true;
            Global::ROBuffer[entry.destROB].value=entry.result;
            Global::instructionQueue[entry.ID_in_Queue].status_in_Queue=InstructionStatus::WB;
            Global::ROBuffer[entry.destROB].dest_physical_register=entry.destPhysicalRegister;
            continue;
        }
        if(!Global::BUQueue.empty()){
            cout<<"-------------------------BUQueue "<<Global::BUQueue.front().ID_in_Queue<<"------------------------"<<endl;
            ReservationStationEntry entry=Global::BUQueue.front();
            Global::ROBuffer[entry.destROB].dest_physical_register=entry.destPhysicalRegister;
            Global::ROBuffer[entry.destROB].done=true;
            std::cout << "[WB] Handling bne ID: " << entry.ID_in_Queue
            << ", destROB: " << entry.destROB
            << ", expect ROB[10]?" << std::endl;
            Global::ROBuffer[entry.destROB].value=entry.result;
            // if(entry.predictTrueFalse.has_value()){
            Global::ROBuffer[entry.destROB].predictTrueFalse=false;
            // }
            //如果有值说明mispredict了，正确预测没有值
            Global::instructionQueue[entry.ID_in_Queue].status_in_Queue=InstructionStatus::WB;
            Global::BUQueue.erase(Global::BUQueue.begin());
            continue;
        }
        if(!Global::StoreQueue.empty()){
            cout<<"-------------------------StoreQueue "<<Global::StoreQueue.front().ID_in_Queue<<"------------------------"<<endl;
            ReadyStore readyStore=Global::StoreQueue.front();
            Global::StoreQueue.erase(Global::StoreQueue.begin());
            Global::ROBuffer[readyStore.rob_id].done=true;
            Global::ROBuffer[readyStore.rob_id].memory_address=readyStore.address;
            Global::ROBuffer[readyStore.rob_id].value=readyStore.value;
            Global::instructionQueue[readyStore.ID_in_Queue].status_in_Queue=InstructionStatus::WB;
            continue;
        }

        if(!Global::LoadQueue.empty()){
            cout<<"-------------------------LoadQueue "<<Global::LoadQueue.front().ID_in_Queue<<"------------------------"<<endl;
            LoadResult loadResult=Global::LoadQueue.front();
            Global::LoadQueue.erase(Global::LoadQueue.begin());
            Global::ROBuffer[loadResult.rob_id].done=true;
            Global::ROBuffer[loadResult.rob_id].value=loadResult.value;
            Global::renaming_worker.physicalRegister[loadResult.prf_id].isReady=true;
            Global::instructionQueue[loadResult.ID_in_Queue].status_in_Queue=InstructionStatus::WB;
            continue;
        }


    }
    cout<<"-------------------------writeBack end------------------------"<<endl;
    //检查PendingLoadQueue里有没有fsd可以forwarding的
    for(auto it = Global::LoadHazardQueue.begin(); it != Global::LoadHazardQueue.end(); ) {
        int already = LoadStore::allFSDReadyAndForwardValue(it->rob_id);
        if(already) {
            optional<double> value = LoadStore::findLatestStoreBeforeWithAddress(it->address, it->rob_id);
            double insert_value;
            if(value.has_value()) {
                writePRF(it->prf_id, value.value()); // 直接 forwarding
                insert_value = value.value();
            } else {
                double value = Global::memory_value[it->address];
                writePRF(it->prf_id, value); // 直接 forwarding
                insert_value = value;
            }
            //加入LoadQueue
            LoadResult loadResult;
            loadResult.rob_id=it->rob_id;
            loadResult.prf_id=it->prf_id;
            loadResult.value=insert_value;
            // Global::renaming_worker.physicalRegister[loadResult.prf_id].isReady=true;
//value.value()是fsd的值，value_memory是memory的值
            LoadStore::insertLoadQueue(loadResult);
            it = Global::LoadHazardQueue.erase(it); // 删除当前元素并返回下一个有效迭代器
        } else {
            ++it; // 如果未处理则继续下一个
        }
    }

    
}

void WriteBack::printCompleteQueue() {
    std::cout << "--------------------- [completeRSQueue] Status ---------------------" << std::endl;
    if (Global::completeRSQueue.empty()) {
        std::cout << "  [EMPTY] No instructions waiting for write-back." << std::endl;
    } else {
        for (const auto& entry : Global::completeRSQueue) {
            std::cout << "  >> ID_in_Queue: " << entry.ID_in_Queue
                      << ", opcode: " << static_cast<int>(entry.opcode)
                      << ", destROB: " << entry.destROB
                      << std::endl;
        }
    }
    std::cout << "--------------------------------------------------------------------" << std::endl;
}