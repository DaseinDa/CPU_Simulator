#include "execute.h"

void Execute::execute(){
    //1. 从ROB中获取指令
    //2. 从RS中获取指令
    //3. 执行指令
    //4. 更新RS和ROB
    PipelineStage INT_pipeline_entry;
    int issue=0;
    if(!Global::RS_INT_Queue.empty()){
        for (auto entry=Global::RS_INT_Queue.begin();entry!=Global::RS_INT_Queue.end() && issue<UNIT_INT_NUMBER;) {
            if(Global::RS_INT_Queue.empty())break;
            if(entry->Qj.empty()&&entry->Qk.empty()){
                Global::instructionQueue[entry->ID_in_Queue].status_in_Queue=InstructionStatus::EXECUTE;
                //发射进INT_pipeline
                INT_pipeline_entry.rs_entry=*entry;
                INT_pipeline_entry.remaining_latency=entry->latency;
                Global::INT_pipeline.push_back(INT_pipeline_entry);
                issue++;
                //先进pipeline才能删除，不然就读不到了，从RS中删除
                entry=Global::RS_INT_Queue.erase(entry);
                break;
            }
            entry++;
        }
    }
    executeINT();//先检查RS有没有ready可加入execute的，然后执行


    PipelineStage LOAD_pipeline_entry;
    if(!Global::RS_LOAD_Queue.empty()){
        for (auto entry=Global::RS_LOAD_Queue.begin();entry!=Global::RS_LOAD_Queue.end() && issue<UNIT_LOAD_NUMBER;) {
            if(Global::RS_LOAD_Queue.empty())break;
            if(entry->Qj.empty()&&entry->Qk.empty()){
                Global::instructionQueue[entry->ID_in_Queue].status_in_Queue=InstructionStatus::EXECUTE;
                //发射进INT_pipeline
                LOAD_pipeline_entry.rs_entry=*entry;
                LOAD_pipeline_entry.remaining_latency=entry->latency;
                Global::LOAD_pipeline.push_back(LOAD_pipeline_entry);
                issue++;
                //先进pipeline才能删除，不然就读不到了，从RS中删除
                entry=Global::RS_LOAD_Queue.erase(entry);
                // executeINT(INT_entry);
                break;
            }
            entry++;
        }
    }
    executeLOAD();



    // if(!Global::RS_LOAD_Queue.empty()){
    //     executeLOAD();
    // }
    // if(!Global::RS_STORE_Queue.empty()){
    //     executeSTORE();
    // }
    // if(!Global::RS_FPadd_Queue.empty()){
    //     executeFPADD();
    // }
    // if(!Global::RS_FPmult_Queue.empty()){
    //     executeFPMULT();
    // }
    // if(!Global::RS_FPdiv_Queue.empty()){
    //     executeFPDIV();
    // }
    // if(!Global::RS_FPBU_Queue.empty()){
    //     executeFPBU();
    // }
}
//每次cycle都要执行executeINT
void Execute::executeINT(){
    //Global::INT_pipeline所有的指令latency-1,因为INT latency=1,所以其实每次INT_pipeline里就只有一条指令
    //INT_pipeline_entry遍历INT_pipeline里所有的指令
    for(auto INT_pipeline_entry=Global::INT_pipeline.begin();INT_pipeline_entry!=Global::INT_pipeline.end();){
        if(Global::INT_pipeline.empty()) break;//没有指令什么都不用执行
        INT_pipeline_entry->remaining_latency--;
        if(INT_pipeline_entry->remaining_latency==0){
            int result=0;
            //运算完成
            //执行指令
            if(INT_pipeline_entry->rs_entry.opcode==add)result=INT_pipeline_entry->rs_entry.Vj+INT_pipeline_entry->rs_entry.Vk;
            else if(INT_pipeline_entry->rs_entry.opcode==addi)result=INT_pipeline_entry->rs_entry.Vj+INT_pipeline_entry->rs_entry.A.value();
            else if(INT_pipeline_entry->rs_entry.opcode==slt)result=INT_pipeline_entry->rs_entry.Vj<INT_pipeline_entry->rs_entry.Vk;
            else{
                throw runtime_error("opcode is not INT");
            }
            INT_pipeline_entry->rs_entry.result=result;
            //写进completeRSqueue,代表计算完成的指令
            insertCompletedEntry(INT_pipeline_entry->rs_entry);
            //ROB和CDB在write back阶段更新
        }
        INT_pipeline_entry++;
    }
}


void Execute::executeLOAD(){
    //Global::LOAD_pipeline所有的指令latency-1,因为LOAD latency=1,所以其实每次LOAD_pipeline里就只有一条指令
    //LOAD_pipeline_entry遍历LOAD_pipeline里所有的指令
    for(auto INT_pipeline_entry=Global::LOAD_pipeline.begin();INT_pipeline_entry!=Global::LOAD_pipeline.end();){
        if(Global::LOAD_pipeline.empty()) break;//没有指令什么都不用执行
        INT_pipeline_entry->remaining_latency--;
        if(INT_pipeline_entry->remaining_latency==0){
            int memory_address=0;
            //运算完成
            //执行指令
            //Vj应该是物理寄存器存的值,能送到execute的都ready了
            if(INT_pipeline_entry->rs_entry.opcode==fld)memory_address=INT_pipeline_entry->rs_entry.Vj+INT_pipeline_entry->rs_entry.A.value();
            else{
                throw runtime_error("opcode is not LOAD");
            }
            //写进completeRSqueue,代表计算完成的指令
            insertCompletedEntry(INT_pipeline_entry->rs_entry);
            //ROB和CDB在write back阶段更新
        }
        INT_pipeline_entry++;
    }
}

void Execute::insertCompletedEntry(const ReservationStationEntry& entry) {
    auto it = std::lower_bound(
        Global::completeRSQueue.begin(),
        Global::completeRSQueue.end(),
        entry,
        [](const ReservationStationEntry& a, const ReservationStationEntry& b) {
            return a.ID_in_Queue < b.ID_in_Queue;  // 升序排序标准
        }
    );

    Global::completeRSQueue.insert(it, entry);  // 插入到正确位置
}
