#include "commit.h"

Commit::Commit(){

}

void Commit::commit(){

while(Global::ROBuffer[Global::robHead].done && Global::robHead!=(Global::robTail+1)%NR){
    ROBEntry entry=Global::ROBuffer[Global::robHead];
    if(entry.opcode==InstructionType::bne){
        bool actualPrediction=get<bool>(entry.value);
        if(entry.predictTrueFalse==false){
            Global::historySnapshot.flush(entry.ID_in_Queue,actualPrediction);
            break;
        }else{
            continue;
        }
    }
    else if(entry.opcode==InstructionType::add || entry.opcode==InstructionType::addi || entry.opcode==InstructionType::slt){
        Global::architectureRegisterFile.at(entry.dest_archi_register.value()).value=get<int>(entry.value);
    }
    else if(entry.opcode==InstructionType::fadd || InstructionType::fsub || InstructionType::fmul || InstructionType::fdiv){
        Global::architectureRegisterFile.at(entry.dest_archi_register.value()).value=get<double>(entry.value);
    }
    else if(entry.opcode==InstructionType::fld){
        ;
    }
    else if(entry.opcode==InstructionType::fsd){
        Global::memory_value[entry.memory_address.value()]=get<double>(entry.value);
    }
    Global::instructionQueue[entry.ID_in_Queue].status_in_Queue=InstructionStatus::COMMIT;
    Global::robHead=(Global::robHead+1)%NR;
}

}