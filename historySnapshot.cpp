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
    snapshot.ID_in_Queue=bne_instruction.ID_in_Queue.value();
    snapshot.current_cycle=Global::current_cycle;
    snapshot.labelMap=Global::labelMap;
    snapshot.memory_value=Global::memory_value;
    snapshot.renaming_worker=Global::renaming_worker;
    snapshot.instructionQueue=Global::instructionQueue;
    snapshot.fetchInstructionQueue=Global::fetchInstructionQueue;   
    snapshot.fetch_pointer=Global::fetch_pointer;
    // snapshot.btb=Global::btb;
    snapshot.architectureRegisterFile=Global::architectureRegisterFile;
    snapshot.decodeInstructionQueue=Global::decodeInstructionQueue;
    snapshot.renameStall=Global::renameStall;
    //Issue
    //ReservationStation
    snapshot.RS_INT_Queue=Global::RS_INT_Queue;
    snapshot.RS_LOAD_Queue=Global::RS_LOAD_Queue;
    snapshot.RS_STORE_Queue=Global::RS_STORE_Queue;
    snapshot.RS_FPadd_Queue=Global::RS_FPadd_Queue;
    snapshot.RS_FPmult_Queue=Global::RS_FPmult_Queue;
    snapshot.RS_FPdiv_Queue=Global::RS_FPdiv_Queue;
    snapshot.RS_BU_Queue=Global::RS_BU_Queue;
    snapshot.rsFullNumber=Global::rsFullNumber;
    snapshot.stallCount_RSFull=Global::stallCount_RSFull;
    //ROB
    snapshot.stallCount_ROBFull=Global::stallCount_ROBFull;
    snapshot.robHead=Global::robHead;
    snapshot.robTail=Global::robTail;
    snapshot.ROBuffer=Global::ROBuffer;
    //unit pipeline
    snapshot.INT_pipeline=Global::INT_pipeline;
    snapshot.LOAD_pipeline=Global::LOAD_pipeline;
    snapshot.STORE_pipeline=Global::STORE_pipeline;
    snapshot.FPadd_pipeline=Global::FPadd_pipeline;
    snapshot.FPmult_pipeline=Global::FPmult_pipeline;
    snapshot.FPdiv_pipeline=Global::FPdiv_pipeline;
    snapshot.BU_pipeline=Global::BU_pipeline;
    //Instructions to complete the calculation
    snapshot.completeRSQueue=Global::completeRSQueue;
    snapshot.BUQueue=Global::BUQueue;
    //load store
    snapshot.LoadQueue=Global::LoadQueue;
    snapshot.LoadHazardQueue=Global::LoadHazardQueue;
    snapshot.StoreQueue=Global::StoreQueue;
    history_snapshots.push_back(snapshot);
}

void HistorySnapshot::predictionTrueFalseRecover(Instruction bne_instruction, bool predictTaken) {
    //This function is called only if the prediction is wrong
    if(bne_instruction.opcode != InstructionType::bne) {
        cout<<"false ID_in_Queue: "<<bne_instruction.ID_in_Queue.value()<<endl;
        throw runtime_error("historySnapshot::predictionTrueFalseRecover() called with non-BNE instruction");
    }
    Snapshot* snapshot = findMatchingSnapshot(bne_instruction.ID_in_Queue.value());

    //No matching snapshots found
    if(snapshot == nullptr) {
        cout<<"false ID_in_Queue: "<<bne_instruction.ID_in_Queue.value()<<endl;
        cout<<"The history_snapshots size is: "<<history_snapshots.size()<<endl;
        throw runtime_error("snapshot not found in the history_snapshots");
    }
    //1. Restore memory_value
    //2. Restore register rename
    //3. Restore fetchInstructionQueue
    //4. Restore instructionset
    //5. Restore fetch_pointerb
    //Get new fetch pointer
    if(!predictTaken){
        int target_position=Global::btb.getTargetPosition(bne_instruction.instructionNumber);
        snapshot->bne_instruction.bne_taken=true;
        Global::fetch_pointer = target_position;
    }else{
        Global::fetch_pointer=bne_instruction.instructionNumber+1;
        snapshot->bne_instruction.bne_taken=false;
    }
    Global::instructionQueue=snapshot->instructionQueue;
    Global::memory_value = snapshot->memory_value;
    Global::renaming_worker = snapshot->renaming_worker;
    Global::fetchInstructionQueue = snapshot->fetchInstructionQueue;
    Global::decodeInstructionQueue=snapshot->decodeInstructionQueue;
    Global::architectureRegisterFile=snapshot->architectureRegisterFile;
    Global::renameStall=snapshot->renameStall;
    //3. Recover ROB
    Global::ROBuffer=snapshot->ROBuffer;
    //4. Restore reservation station
    Global::RS_INT_Queue=snapshot->RS_INT_Queue;
    Global::RS_LOAD_Queue=snapshot->RS_LOAD_Queue;
    Global::RS_STORE_Queue=snapshot->RS_STORE_Queue;
    Global::RS_FPadd_Queue=snapshot->RS_FPadd_Queue;
    Global::RS_FPmult_Queue=snapshot->RS_FPmult_Queue;
    Global::RS_FPdiv_Queue=snapshot->RS_FPdiv_Queue;
    Global::RS_BU_Queue=snapshot->RS_BU_Queue;
    //5. Recovering the ROB
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
    Global::StoreQueue=snapshot->StoreQueue;
    //5. Restore decode
    //6. Restore issue
    //7. Restore execute
    //8. Restore write back
    //9. Restore commit
    //The snapshot is stored when the pointer points to bne. Restoring the snapshot will make the fetch pointer point to bne again. Here, the fetch pointer needs to be updated according to the actual branch result
    //After the rollback is completed, the predictor state will also roll back to the original state, so the result of the bne prediction can be known based on the original branch predictor state
    if(!predictTaken) {
        //Actual taken
        Global::fetch_pointer = snapshot->btb.getTargetPosition(bne_instruction.instructionNumber);//This is still retained, because it is agreed to index entry according to address4-7
    } else {
        //Actual not taken
        Global::fetch_pointer++;
    }   

    //Prediction error, the path after the current bne is wrong, the program sequence is no longer used, delete all snapshots after the current snapshot snap
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
    clearHistoryAfter(ID_in_Queue);
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
    Global::LoadQueue.clear();
    Global::fetch_pointer=0;
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
    Global::StoreQueue.clear();
    //Roll back RAT, register value instructionQueue, set fetch pointer
    Global::memory_value = snapshot->memory_value;
    Global::renaming_worker = snapshot->renaming_worker;
    Global::rsFullNumber=snapshot->rsFullNumber;
    Global::stallCount_RSFull=snapshot->stallCount_RSFull;
    Global::stallCount_ROBFull=snapshot->stallCount_ROBFull;
    Global::instructionQueue=snapshot->instructionQueue;
    Global::stallCount_ROBFull=snapshot->stallCount_ROBFull;
    Global::renameStall=snapshot->renameStall;

}

Snapshot* HistorySnapshot::findMatchingSnapshot(int ID_in_Queue) {
    for (auto& snap : history_snapshots) {
        cout<<"snap.ID_in_Queue: "<<snap.bne_instruction.ID_in_Queue.value()<<endl;
        if (snap.bne_instruction.ID_in_Queue.value() == ID_in_Queue) {
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
        history_snapshots.erase(next(it), history_snapshots.end());
    }
}
