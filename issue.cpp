#include "issue.h"

Issue::Issue(){
}

Issue::~Issue(){
}

void Issue::issue(){
    cout<<"Issue......"<<endl;
    bool rsStallThisCycle=false;
    bool robStallThisCycle=false;

    //每次阶段仍然尝试从长度为4的issueInstruction取指
    for(int i=0;i<nw;i++){
        if(Global::decodeInstructionQueue.empty()){
            break;
        }
        Instruction inst=Global::decodeInstructionQueue.front();
        //分配ROB,还是要写按opcode解析成ROBEntry
        //如果robHead和robTail相等，说明ROB满了
        if(inst.ID_in_Queue.value()%NR==Global::robHead){
            robStallThisCycle=true;
        }
        //发射到reservationStation,如果全部RS avaialable, issue就发射四条。如果RS满了那每次issue只能发射<4条或者不发射，剩下等待到下一个cycle
        //reservationStatin不需要判断issueInstructionQueue是否空，因为rsStall本身只在rs满的时候count
        //issue本身不算一个cycle,加入issueInstructionQueue可以马上发射到rs,不应该受issue capacity影响
        switch(inst.opcode){
            case InstructionType::add:
            case InstructionType::addi:
            case InstructionType::slt:
                if(Global::RS_INT_Queue.size()>=RS_INT_SIZE){
                    cout<<"Issue: RS_INT is full"<<endl;
                    rsStallThisCycle=true;
                    Global::rsFullNumber++;
                }
                break;
            case InstructionType::fld:
                if(Global::RS_LOAD_Queue.size()>=RS_LOAD_SIZE){
                    cout<<"Issue: RS_LOAD is full"<<endl;
                    rsStallThisCycle=true;
                    Global::rsFullNumber++;
                }
                break;
            case InstructionType::fsd:
                if(Global::RS_STORE_Queue.size()<RS_STORE_SIZE){
                    cout<<"Issue: RS_STORE is full"<<endl;
                    rsStallThisCycle=true;
                    Global::rsFullNumber++;
                }
                break;
            case InstructionType::fadd:
            case InstructionType::fsub:
                if(Global::RS_FPadd_Queue.size()<RS_FPADD_SIZE){
                    cout<<"Issue: RS_FPadd is full"<<endl;
                    rsStallThisCycle=true;
                    Global::rsFullNumber++;
                }
                break;
            case InstructionType::fmul:
                if(Global::RS_FPmult_Queue.size()<RS_FPMULT_SIZE){
                    cout<<"Issue: RS_FPmult is full"<<endl;
                    rsStallThisCycle=true;
                    Global::rsFullNumber++;
                }   
                break;
            case InstructionType::fdiv:
                if(Global::RS_FPdiv_Queue.size()<RS_FPDIV_SIZE){
                    cout<<"Issue: RS_FPmult is full"<<endl;
                    rsStallThisCycle=true;
                    Global::rsFullNumber++;
                }
                break;
            case InstructionType::bne:
                if(Global::RS_BU_Queue.size()<RS_FPBU_SIZE){
                    cout<<"Issue: RS_BU is full"<<endl;
                    rsStallThisCycle=true;
                    Global::rsFullNumber++;  
                }
                break;
            default:
                cout<<"Issue: Unknown instruction"<<endl;
                Global::rsFullNumber++;
                break;
        }
        if (rsStallThisCycle || robStallThisCycle) {
            if (rsStallThisCycle) Global::stallCount_RSFull++;
            if (robStallThisCycle) Global::stallCount_ROBFull++;
            break; // 当前周期资源不足，跳出 issue loop
        }
        if(!rsStallThisCycle && !robStallThisCycle){
            Global::decodeInstructionQueue.pop_front();
            Global::instructionQueue[inst.ID_in_Queue.value()].status_in_Queue=InstructionStatus::ISSUE;
            //分配ROB
            //bne和fsd的rob条目需要特殊处理，因为它们没有终点寄存器
            if(inst.opcode==InstructionType::bne || inst.opcode==InstructionType::fsd){
                ROBEntry rob_entry;
                rob_entry.ID_in_Queue=inst.ID_in_Queue.value();
                rob_entry.opcode=inst.opcode;
                rob_entry.status=InstructionStatus::ISSUE;
                Global::robTail=inst.ID_in_Queue.value()%NR;
                Global::ROBuffer.at(Global::robTail)=rob_entry;
            }else{
                ROBEntry rob_entry;
                rob_entry.ID_in_Queue=inst.ID_in_Queue.value();
                rob_entry.opcode=inst.opcode;
                rob_entry.status=InstructionStatus::ISSUE;
                rob_entry.dest_archi_register=Global::instructionQueue[inst.ID_in_Queue.value()].operands[0];
                rob_entry.dest_physical_register=inst.operands[0];//现在已经是物理寄存器指令了
                //现在终点寄存器还没有值value
                Global::robTail=inst.ID_in_Queue.value()%NR;
                Global::ROBuffer.at(Global::robTail)=rob_entry;
            }

                //分配RS
                //解析成RS数据结构
                ReservationStationEntry rs_entry;
                inst.print();
                rs_entry.opcode=inst.opcode;
                rs_entry.Vj=inst.Vj;
                rs_entry.Vk=inst.Vk;
                rs_entry.Qj=inst.Qj;
                rs_entry.Qk=inst.Qk;
                rs_entry.destPhysicalRegister=inst.operands[0];//如果维护这个的话，需要区分没有终点寄存器的bne和fsd
                rs_entry.destROB=Global::robTail;
                rs_entry.latency=Unit::getInstrLatency(inst.opcode);
                rs_entry.issueCycle=Global::current_cycle;
                rs_entry.A=inst.immediate;
                rs_entry.ID_in_Queue=inst.ID_in_Queue.value();
                insert2RS(rs_entry);

        }
}
    //forwarding的原子操作之一，检查有没有之前execute阶段forwarding的指令
    checkRSOperandsReady();
}

void Issue::insert2RS(ReservationStationEntry& rs_entry){
    switch(rs_entry.opcode){
                case InstructionType::add:
            case InstructionType::addi:
            case InstructionType::slt:
                Global::RS_INT_Queue.push_back(rs_entry);
            case InstructionType::fld:
                Global::RS_LOAD_Queue.push_back(rs_entry);
            case InstructionType::fsd:
                Global::RS_STORE_Queue.push_back(rs_entry);
            case InstructionType::fadd:
            case InstructionType::fsub:
                Global::RS_FPadd_Queue.push_back(rs_entry);
            case InstructionType::fmul:
                    Global::RS_FPmult_Queue.push_back(rs_entry);
            case InstructionType::fdiv:
                    Global::RS_FPdiv_Queue.push_back(rs_entry);
            case InstructionType::bne:
                    Global::RS_BU_Queue.push_back(rs_entry);
            default:
                cout<<"Issue: Unknown instruction"<<endl;
                Global::rsFullNumber++;
                break;
        }

}