#include "historySnapshot.h"

HistorySnapshot::HistorySnapshot() {
    history_snapshots = deque<Snapshot>();
}
HistorySnapshot::~HistorySnapshot(){
    cout<<"HistorySnapshot destructor called"<<endl;
}
void HistorySnapshot::addSnapshot(Instruction bne_instruction) {
    Snapshot snapshot;
    if(bne_instruction.opcode != InstructionType::bne) {
        throw runtime_error("addSnapshot() called with non-BNE instruction");
    }
    snapshot.bne_instruction = bne_instruction;
    snapshot.memory_value = Global::memory_value;
    snapshot.renaming_worker = Global::renaming_worker;
    snapshot.instructionset = Global::instructionset;
    snapshot.fetchInstructionQueue = Global::fetchInstructionQueue;
    snapshot.fetch_pointer = Global::fetch_pointer;
    snapshot.btb = Global::btb; 
    history_snapshots.push_back(snapshot);
}

void HistorySnapshot::predictionTrueFalseRecover(Instruction bne_instruction, bool actucalPrediction) {
    //只有预测错误才会调用这个函数
    if(bne_instruction.opcode != InstructionType::bne) {
        throw runtime_error("predictionTrueFalse() called with non-BNE instruction");
    }
    Snapshot* snapshot = findMatchingSnapshot(bne_instruction.ID_in_Queue.value());

    //预测错误，恢复快照
    if(snapshot == nullptr) {
        throw runtime_error("predictionTrueFalse() called with non-BNE instruction");
    }
    //1. 恢复memory_value
    //2. 恢复寄存器重命名
    //3. 恢复fetchInstructionQueue
    //4. 恢复instructionset
    //5. 恢复fetch_pointer
    //6. 恢复btb
    //得到新的fetch pointer
    if(actucalPrediction){
        int target_position=Global::btb.getTargetPosition(bne_instruction.instructionNumber);
        Global::fetch_pointer = target_position;
    }else{
        Global::fetch_pointer++;
    }
    Global::instructionQueue=snapshot->instructionQueue;
    Global::memory_value = snapshot->memory_value;
    Global::renaming_worker = snapshot->renaming_worker;
    Global::instructionset = snapshot->instructionset;
    Global::fetchInstructionQueue = snapshot->fetchInstructionQueue;
    Global::decodeInstructionQueue=snapshot->decodeInstructionQueue;
    Global::historySnapshot=snapshot->historySnapshot;
    Global::architectureRegisterFile=snapshot->architectureRegisterFile;
    Global::renameStall=snapshot->renameStall;
    Global::dependency_map=snapshot->dependency_map;
    //3. 恢复ROB
    Global::ROBuffer=snapshot->ROBuffer;
    //4. 恢复reservation station
    Global::RS_INT_Queue=snapshot->RS_INT_Queue;
    Global::RS_LOAD_Queue=snapshot->RS_LOAD_Queue;
    Global::RS_STORE_Queue=snapshot->RS_STORE_Queue;
    Global::RS_FPadd_Queue=snapshot->RS_FPadd_Queue;
    Global::RS_FPmult_Queue=snapshot->RS_FPmult_Queue;
    Global::RS_FPdiv_Queue=snapshot->RS_FPdiv_Queue;
    Global::RS_BU_Queue=snapshot->RS_BU_Queue;
    //5. 恢复ROB
    Global::stallCount_ROBFull=snapshot->stallCount_ROBFull;
    Global::robHead=snapshot->robHead;
    Global::robTail=snapshot->robTail;
    Global::ROBuffer=snapshot->ROBuffer;
    //unit pipeline
    Global::INT_pipeline=snapshot->INT_pipeline;
    Global::LOAD_pipeline=snapshot->LOAD_pipeline;
    Global::STORE_pipeline=snapshot->STORE_pipeline;
    Global::FPadd_pipeline=snapshot->FPadd_pipeline;
    Global::FPmult_pipeline=snapshot->FPmult_pipeline;
    Global::FPdiv_pipeline=snapshot->FPdiv_pipeline;
    Global::BU_pipeline=snapshot->BU_pipeline;

    Global::completeRSQueue=snapshot->completeRSQueue;
    Global::BUQueue=snapshot->BUQueue;
    Global::LoadQueue=snapshot->LoadQueue;
    Global::LoadHazardQueue=snapshot->LoadHazardQueue;
    Global::LoadResultQueue=snapshot->LoadResultQueue;
    Global::StoreQueue=snapshot->StoreQueue;
    //5. 恢复decode
    //6. 恢复issue
    //7. 恢复execute
    //8. 恢复write back
    //9. 恢复commit
    //快照是在pointer指向bne时存储的，恢复快照会使得fetch pointer重新指向bne,这里要根据实际分支结果来更新fetch pointer
    //回滚完成后 预测器状态也会回滚到当初的状态，所以根据当初的分支预测器状态就能知道该bne预测时的结果
    if(actucalPrediction) {
        //Actual taken
        Global::fetch_pointer = snapshot->btb.getTargetPosition(bne_instruction.instructionNumber);//这个还是保留，因为说好根据address4-7索引entry
    } else {
        //Actual not taken
        Global::fetch_pointer++;
    }   

    //预测错误，当前bne后的路径错误，程序顺序不再使用，删除从现在现在的快照snap后的所有快照
    clearHistoryAfter(bne_instruction.ID_in_Queue.value());
//     auto it = std::find_if(history_snapshots.begin(), history_snapshots.end(),
//         [snapshot](const Snapshot& snap) { return &snap == snapshot; });
//     if (it != history_snapshots.end()) {
//         history_snapshots.erase(it, history_snapshots.end());
//     }

}
void HistorySnapshot::flush(int ID_in_Queue, bool actucalPrediction){
    if(actucalPrediction){
        int instructionNumber=Global::instructionQueue[ID_in_Queue].instructionNumber;
        int target_position=Global::btb.getTargetPosition(instructionNumber);
        Global::fetch_pointer = target_position;
    }else{
            Global::fetch_pointer++;
    }
    Snapshot* snapshot = findMatchingSnapshot(ID_in_Queue);
    Global::fetchInstructionQueue.clear();
    Global::ROBuffer.clear();
    Global::RS_INT_Queue.clear();
    Global::RS_LOAD_Queue.clear();
    Global::RS_STORE_Queue.clear();
    Global::RS_FPadd_Queue.clear();
    Global::RS_FPmult_Queue.clear();
    Global::RS_FPdiv_Queue.clear();
    Global::RS_BU_Queue.clear();
    Global::completeRSQueue.clear();
    Global::BUQueue.clear();
    Global::LoadQueue.clear();
    Global::LoadHazardQueue.clear();
    Global::LoadResultQueue.clear();
    Global::fetch_pointer=0;
    Global::historySnapshot.clear();
    Global::robHead=0;
    Global::robTail=0;
    Global::ROBuffer.clear();
    Global::INT_pipeline.clear();
    Global::LOAD_pipeline.clear();
    Global::STORE_pipeline.clear();
    Global::FPadd_pipeline.clear();
    Global::FPmult_pipeline.clear();
    Global::FPdiv_pipeline.clear();
    Global::BU_pipeline.clear();
    Global::completeRSQueue.clear();
    Global::BUQueue.clear();
    Global::LoadQueue.clear();
    Global::LoadHazardQueue.clear();
    Global::LoadResultQueue.clear();
    //回滚RAT,寄存器值instructionQueue,设置fetch pointer
    Global::memory_value = snapshot->memory_value;
    Global::renaming_worker = snapshot->renaming_worker;
    Global::rsFullNumber=snapshot->rsFullNumber;
    Global::stallCount_RSFull=snapshot->stallCount_RSFull;
    Global::stallCount_ROBFull=snapshot->stallCount_ROBFull;
    Global::instructionQueue=snapshot->instructionQueue;
    Global::stallCount_ROBFull=snapshot->stallCount_ROBFull;
    Global::renameStall=snapshot->renameStall;
    Global::dependency_map=snapshot->dependency_map;
}

Snapshot* HistorySnapshot::findMatchingSnapshot(int ID_in_Queue) {
    for (auto& snap : history_snapshots) {
        if (snap.bne_instruction.ID_in_Queue == ID_in_Queue) {
            return &snap;
        }
    }
    return nullptr;
}
void HistorySnapshot::clearHistoryAfter(int ID_in_Queue) {
    auto it = std::find_if(
        history_snapshots.begin(),
        history_snapshots.end(),
        [&](const Snapshot& snap) {
            return snap.bne_instruction.ID_in_Queue == ID_in_Queue;
        }
    );

    if (it != history_snapshots.end()) {
        // 删除 it 之后（含 it）的所有 snapshot
        history_snapshots.erase(it, history_snapshots.end());
    }
}
