#include "issue.h"

Issue::Issue(){
}

Issue::~Issue(){
}

bool Issue::issue(){
    cout<<"Issue......"<<endl;
    int startIssueQueueSize=Global::issueInstructionQueue.size();
    int capacity=nw-startIssueQueueSize;
    bool rsStallThisCycle=false;
    bool robStallThisCycle=false;
    if(capacity<=0){
        cout<<"Issue: No capacity"<<endl;//没有capacity可以issue不做任何操作
    }else{
        for(int i=0;i<capacity;i++){
            if(Global::decodeInstructionQueue.empty()){
                break;//decodeInstructionQueue为空，说明issue阶段本身没有引起stall
            }
            Instruction inst=Global::decodeInstructionQueue.front();
            inst.status_in_Queue=ISSUE;
            Global::decodeInstructionQueue.pop_front();
            Global::issueInstructionQueue.push_back(inst);
        }
    }
    //每次阶段仍然尝试从长度为4的issueInstruction取指
    for(int i=0;i<nw;i++){
        if(Global::issueInstructionQueue.empty()){
            break;
        }
        Instruction inst=Global::issueInstructionQueue.front();
        Global::issueInstructionQueue.pop_front();
        //解析成RS数据结构
        ReservationStationEntry rs_entry;
        inst.print();
        rs_entry.opcode=inst.opcode;
        rs_entry.Vj=inst.Vj;
        rs_entry.Vk=inst.Vk;
        rs_entry.Qj=inst.Qj;
        rs_entry.Qk=inst.Qk;
        rs_entry.dest=inst.operands[0];
        rs_entry.latency=Unit::getInstrLatency(inst.opcode);
        rs_entry.issueCycle=Global::current_cycle;
        rs_entry.A=inst.immediate;
        rs_entry.ID_in_Queue=inst.ID_in_Queue.value();
        //发射到reservationStation,如果全部RS avaialable, issue就发射四条。如果RS满了那每次issue只能发射<4条或者不发射，剩下等待到下一个cycle
        //reservationStatin不需要判断issueInstructionQueue是否空，因为rsStall本身只在rs满的时候count
        //issue本身不算一个cycle,加入issueInstructionQueue可以马上发射到rs,不应该受issue capacity影响
        switch(inst.opcode){
            case InstructionType::add:
            case InstructionType::addi:
            case InstructionType::slt:
                if(Global::RS_INT_Queue.size()<RS_INT_SIZE){
                    Global::instructionQueue[inst.ID_in_Queue.value()].status_in_Queue=RS_INT;//维护跟踪所有程序顺序指令的状态
                    rs_entry.rsCycle=Global::current_cycle;
                    Global::RS_INT_Queue.push_back(rs_entry);//RS_INT和Instruction数据结构不同，要在这里解析成RS形式
                }
                else{
                    cout<<"Issue: RS_INT is full"<<endl;
                    rsStallThisCycle=true;
                    Global::rsFullNumber++;
                    break;
                }
            case InstructionType::fld:
                if(Global::RS_LOAD_Queue.size()<RS_LOAD_SIZE){
                    Global::instructionQueue[inst.ID_in_Queue.value()].status_in_Queue=RS_LOAD;
                    rs_entry.rsCycle=Global::current_cycle;
                    Global::RS_LOAD_Queue.push_back(rs_entry);
                }
                else{
                    cout<<"Issue: RS_LOAD is full"<<endl;
                    rsStallThisCycle=true;
                    Global::rsFullNumber++;
                    break;
                }
            case InstructionType::fsd:
                if(Global::RS_STORE_Queue.size()<RS_STORE_SIZE){
                    Global::instructionQueue[inst.ID_in_Queue.value()].status_in_Queue=RS_STORE;
                    rs_entry.rsCycle=Global::current_cycle;
                    Global::RS_STORE_Queue.push_back(rs_entry);
                }else{
                    cout<<"Issue: RS_STORE is full"<<endl;
                    rsStallThisCycle=true;
                    Global::rsFullNumber++;
                    break;
                }
            case InstructionType::fadd:
            case InstructionType::fsub:
                if(Global::RS_FPadd_Queue.size()<RS_FPADD_SIZE){
                    Global::instructionQueue[inst.ID_in_Queue.value()].status_in_Queue=RS_FPADD;
                    rs_entry.rsCycle=Global::current_cycle;
                    Global::RS_FPadd_Queue.push_back(rs_entry);
                }else{
                    cout<<"Issue: RS_FPadd is full"<<endl;
                    rsStallThisCycle=true;
                    Global::rsFullNumber++;
                    break;
                }
            case InstructionType::fmul:
                if(Global::RS_FPmult_Queue.size()<RS_FPMULT_SIZE){
                    Global::instructionQueue[inst.ID_in_Queue.value()].status_in_Queue=RS_FPMULT;
                    rs_entry.rsCycle=Global::current_cycle;
                    Global::RS_FPmult_Queue.push_back(rs_entry);
                }else{
                    cout<<"Issue: RS_FPmult is full"<<endl;
                    rsStallThisCycle=true;
                    Global::rsFullNumber++;
                    break;
                }   
            case InstructionType::fdiv:
                if(Global::RS_FPdiv_Queue.size()<RS_FPDIV_SIZE){
                    Global::instructionQueue[inst.ID_in_Queue.value()].status_in_Queue=RS_FPDIV;
                    rs_entry.rsCycle=Global::current_cycle;
                    Global::RS_FPdiv_Queue.push_back(rs_entry);
                }else{
                    cout<<"Issue: RS_FPmult is full"<<endl;
                    rsStallThisCycle=true;
                    Global::rsFullNumber++;
                    break;
                }
            case InstructionType::bne:
                if(Global::RS_FPBU_Queue.size()<RS_FPBU_SIZE){
                    Global::instructionQueue[inst.ID_in_Queue.value()].status_in_Queue=RS_FPBU;
                    rs_entry.rsCycle=Global::current_cycle;
                    Global::RS_FPBU_Queue.push_back(rs_entry);
                }else{
                    cout<<"Issue: RS_FPBU is full"<<endl;
                    rsStallThisCycle=true;
                    Global::rsFullNumber++;  
                    break;
                }
            default:
                cout<<"Issue: Unknown instruction"<<endl;
                Global::rsFullNumber++;
                break;
        }
    }
    if(rsStallThisCycle){
        Global::stallCount_RSFull++;
    }

    

    return Global::decodeInstructionQueue.empty() || startIssueQueueSize==0;
}