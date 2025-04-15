#include "commit.h"

Commit::Commit(){

}
//释放物理寄存器
void Commit::commit(){
cout<<"-------------------------commit start------------------------"<<endl;

while(Global::ROBuffer[Global::robHead].done && Global::robHead!=(Global::robTail+1)%NR){
    ROBEntry entry=Global::ROBuffer[Global::robHead];
    cout<<"-------------------------commit "<<Global::robHead<<" done: "<<Global::ROBuffer[Global::robHead].done<<" ID QUEUE: " <<entry.ID_in_Queue<<"------------------------"<<endl;
    if(entry.opcode==InstructionType::bne){
        bool actualPrediction=get<bool>(entry.value);
        if(entry.predictTrueFalse==false){
            cout<<"-----------------------------------------------------commit bne flush------------------------"<<endl;
            Global::historySnapshot.flush(entry.ID_in_Queue,actualPrediction);
            break;
        }
        // Global::renaming_worker.physicalRegister.erase(entry.dest_physical_register.value());
        // Global::renaming_worker.RegisterRenamingFreeList.push_back(entry.dest_physical_register.value());
    }
    else if(entry.opcode==InstructionType::add || entry.opcode==InstructionType::addi || entry.opcode==InstructionType::slt){
        // Global::renaming_worker.physicalRegister[entry.dest_physical_register.value()].isReady=false;
        Global::renaming_worker.registerRenamingMapping.erase(entry.dest_archi_register.value());
        Global::renaming_worker.RegisterRenamingFreeList.push_back(entry.dest_physical_register.value());
        Global::architectureRegisterFile.at(entry.dest_archi_register.value()).value=get<int>(entry.value);
        // Global::architectureRegisterFile.at(entry.dest_archi_register.value()).isReady=true;
        cout<<"%%%%%%%%%%%%%%%commit int "<<entry.dest_archi_register.value()<<" "<<get<int>(entry.value)<<" "<<entry.dest_physical_register.value()<<endl;
    }
    else if(entry.opcode == InstructionType::fsub ||
    entry.opcode == InstructionType::fmul ||
    entry.opcode == InstructionType::fdiv){
        // Global::renaming_worker.physicalRegister.erase(entry.dest_physical_register.value());
        // Global::renaming_worker.physicalRegister[entry.dest_physical_register.value()].isReady=false;
        Global::renaming_worker.registerRenamingMapping.erase(entry.dest_archi_register.value());
        Global::renaming_worker.RegisterRenamingFreeList.push_back(entry.dest_physical_register.value());
        Global::architectureRegisterFile.at(entry.dest_archi_register.value()).value=get<double>(entry.value);
        // Global::architectureRegisterFile.at(entry.dest_archi_register.value()).=true;
    }
    else if(entry.opcode==InstructionType::fld){
        // Global::renaming_worker.physicalRegister.erase(entry.dest_physical_register.value());
        Global::renaming_worker.registerRenamingMapping.erase(entry.dest_archi_register.value());
        Global::architectureRegisterFile.at(entry.dest_archi_register.value()).value=get<double>(entry.value);
        // Global::renaming_worker.physicalRegister[entry.dest_physical_register.value()].isReady=false;
        Global::renaming_worker.RegisterRenamingFreeList.push_back(entry.dest_physical_register.value());
    }
    else if(entry.opcode==InstructionType::fsd){
        Global::memory_value[entry.memory_address.value()]=get<double>(entry.value);
    }
    Global::instructionQueue[entry.ID_in_Queue].status_in_Queue=InstructionStatus::COMMIT;
    Global::ROBuffer[Global::robHead] = ROBEntry();
    Global::robHead=(Global::robHead+1)%NR;
}

}