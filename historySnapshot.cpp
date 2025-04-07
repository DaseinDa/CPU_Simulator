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
    if(bne_instruction.opcode != InstructionType::bne) {
        throw runtime_error("predictionTrueFalse() called with non-BNE instruction");
    }
    Snapshot* snapshot = findMatchingSnapshot(bne_instruction.instructionNumber, bne_instruction.executionCount);

    if(actucalPrediction==snapshot->btb.getPrediction(bne_instruction.instructionNumber)) {
        //预测正确，因为程序顺序不改变，对历史快照不做任何操作
    } else {
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
        Global::memory_value = snapshot->memory_value;
        Global::renaming_worker = snapshot->renaming_worker;
        Global::instructionset = snapshot->instructionset;
        Global::fetchInstructionQueue = snapshot->fetchInstructionQueue;
        Global::fetch_pointer = snapshot->fetch_pointer;
        Global::btb = snapshot->btb;
        //3. 恢复ROB
        //4. 恢复reservation station
        //5. 恢复decode
        //6. 恢复issue
        //7. 恢复execute
        //8. 恢复write back
        //9. 恢复commit
        //快照是在pointer指向bne时存储的，恢复快照会使得fetch pointer重新指向bne,这里要根据实际分支结果来更新fetch pointer
        //回滚完成后 预测器状态也会回滚到当初的状态，所以根据当初的分支预测器状态就能知道该bne预测时的结果
        if(actucalPrediction) {
            //Actual taken
            Global::fetch_pointer = snapshot->btb.getTargetPosition(bne_instruction.instructionNumber);
        } else {
            //Actual not taken
            Global::fetch_pointer++;
        }   

        //预测错误，当前bne后的路径错误，程序顺序不再使用，删除从现在现在的快照snap后的所有快照
        auto it = std::find_if(history_snapshots.begin(), history_snapshots.end(),
            [snapshot](const Snapshot& snap) { return &snap == snapshot; });
        if (it != history_snapshots.end()) {
            history_snapshots.erase(it, history_snapshots.end());
        }
    }
}
Snapshot* HistorySnapshot::findMatchingSnapshot(int instruction_number, int execution_count) {
    for (auto& snap : history_snapshots) {
        if (snap.bne_instruction.instructionNumber == instruction_number &&
            snap.bne_instruction.executionCount == execution_count) {
            return &snap;
        }
    }
    return nullptr;
}