#include "execute.h"

bool Execute::execute(){
    //1. 从ROB中获取指令
    //2. 从RS中获取指令
    //3. 执行指令
    //4. 更新RS和ROB
    //先运行检验bne，这样能减少不必要的计算量
    int issue=0;
    PipelineStage BU_pipeline_entry;
    //找到当前BU RS最小的ID_in_Queue最早的指令，不管是否ready
    int earliest_ID_in_Queue=getEarliestIDIn_RS_BU_Queue();
    cout<<"RS BU executing...... "<<endl;
    //如果RS_BU_Queue里没有ready的指令，则不执行
    if(!Global::RS_BU_Queue.empty()){
        for(issue=0;issue<UNIT_BU_NUMBER;issue++){
            //从ready的指令中找到最早的指令优先执行
            if(Global::RS_BU_Queue.empty()) cout<<"No instruction in BU Reservation Station"<<endl; break;
            auto it = std::min_element(
                Global::RS_BU_Queue.begin(), Global::RS_BU_Queue.end(),
                [](const ReservationStationEntry& a, const ReservationStationEntry& b) {
                    if (!a.Qj.empty() || a.Qk.empty()) return false;
                    if (b.Qj.empty() || b.Qk.empty()) return true;
                    return a.ID_in_Queue < b.ID_in_Queue;
                }
            );
            BU_pipeline_entry.rs_entry=*it;
            BU_pipeline_entry.remaining_latency=it->latency;
            Global::BU_pipeline.push_back(BU_pipeline_entry);
            issue++;
            //先进pipeline才能删除，不然就读不到了，从RS中删除
            it=Global::RS_BU_Queue.erase(it);
            break;
            cout<<"The ready earliest ID_in_Queue in RS_BU_Queue is: "<<it->ID_in_Queue<<endl;
        }
    }
    bool predictFalsestall=executeBU(earliest_ID_in_Queue);
    if(!predictFalsestall) return false;



    PipelineStage INT_pipeline_entry;
    issue=0;
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
    issue=0;
    if(!Global::RS_LOAD_Queue.empty()){
        for (auto entry=Global::RS_LOAD_Queue.begin();entry!=Global::RS_LOAD_Queue.end() && issue<UNIT_LOAD_NUMBER;) {
            if(Global::RS_LOAD_Queue.empty())break;
            if(entry->Qj.empty()&&entry->Qk.empty()){
                Global::instructionQueue[entry->ID_in_Queue].status_in_Queue=InstructionStatus::EXECUTE;
                //发射进LOAD_pipeline
                LOAD_pipeline_entry.rs_entry=*entry;
                LOAD_pipeline_entry.remaining_latency=entry->latency;
                Global::LOAD_pipeline.push_back(LOAD_pipeline_entry);
                issue++;
                //先进pipeline才能删除，不然就读不到了，从RS中删除
                entry=Global::RS_LOAD_Queue.erase(entry);
                break;
            }
            entry++;
        }
    }
    executeLOAD();


    PipelineStage STORE_pipeline_entry;
    issue=0;
    if(!Global::RS_STORE_Queue.empty()){
        for (auto entry=Global::RS_STORE_Queue.begin();entry!=Global::RS_STORE_Queue.end() && issue<UNIT_STORE_NUMBER;) {
            if(Global::RS_STORE_Queue.empty())break;
            if(entry->Qj.empty()&&entry->Qk.empty()){
                Global::instructionQueue[entry->ID_in_Queue].status_in_Queue=InstructionStatus::EXECUTE;
                //发射进STORE_pipeline
                STORE_pipeline_entry.rs_entry=*entry;
                STORE_pipeline_entry.remaining_latency=entry->latency;
                Global::STORE_pipeline.push_back(STORE_pipeline_entry);
                issue++;
                //先进pipeline才能删除，不然就读不到了，从RS中删除
                entry=Global::RS_STORE_Queue.erase(entry);
                break;
            }
            entry++;
        }
    }
    executeSTORE();



    PipelineStage FPADD_pipeline_entry;
    issue=0;
    if(!Global::RS_FPadd_Queue.empty()){
        for (auto entry=Global::RS_FPadd_Queue.begin();entry!=Global::RS_FPadd_Queue.end() && issue<UNIT_FPADD_NUMBER;) {
            if(Global::RS_FPadd_Queue.empty())break;
            if(entry->Qj.empty()&&entry->Qk.empty()){
                Global::instructionQueue[entry->ID_in_Queue].status_in_Queue=InstructionStatus::EXECUTE;
                //发射进FPADD_pipeline
                FPADD_pipeline_entry.rs_entry=*entry;
                FPADD_pipeline_entry.remaining_latency=entry->latency;
                Global::FPadd_pipeline.push_back(FPADD_pipeline_entry);
                issue++;
                //先进pipeline才能删除，不然就读不到了，从RS中删除
                entry=Global::RS_FPadd_Queue.erase(entry);
                break;
            }
            entry++;
        }
    }
    executeFPADD();

//FPmult是non pipelined,从这里要注意了.如果FPmult每次只能读一条指令，如果里面pipeline里有指令，就不能插入新的RS execute了
    PipelineStage FPmult_pipeline_entry;
    if(!Global::RS_FPmult_Queue.empty()){
        for (auto entry=Global::RS_FPmult_Queue.begin();entry!=Global::RS_FPmult_Queue.end() && Global::FPmult_pipeline.size()<UNIT_FPMULT_NUMBER;) {
            if(Global::RS_FPmult_Queue.empty())break;
            if(entry->Qj.empty()&&entry->Qk.empty()){
                Global::instructionQueue[entry->ID_in_Queue].status_in_Queue=InstructionStatus::EXECUTE;
                //发射进FPADD_pipeline
                FPmult_pipeline_entry.rs_entry=*entry;
                FPmult_pipeline_entry.remaining_latency=entry->latency;
                Global::FPmult_pipeline.push_back(FPmult_pipeline_entry);
                //先进pipeline才能删除，不然就读不到了，从RS中删除
                entry=Global::RS_FPmult_Queue.erase(entry);
                break;
            }
            entry++;
        }
    }
    executeFPMULT();
//FPdiv
    PipelineStage FPdiv_pipeline_entry;
    if(!Global::RS_FPdiv_Queue.empty()){
        for (auto entry=Global::RS_FPdiv_Queue.begin();entry!=Global::RS_FPdiv_Queue.end() && Global::FPdiv_pipeline.size()<UNIT_FPDIV_NUMBER;) {
            if(Global::RS_FPdiv_Queue.empty())break;
            if(entry->Qj.empty()&&entry->Qk.empty()){
                Global::instructionQueue[entry->ID_in_Queue].status_in_Queue=InstructionStatus::EXECUTE;
                //发射进FPADD_pipeline
                FPdiv_pipeline_entry.rs_entry=*entry;
                FPdiv_pipeline_entry.remaining_latency=entry->latency;
                Global::FPdiv_pipeline.push_back(FPdiv_pipeline_entry);
                //先进pipeline才能删除，不然就读不到了，从RS中删除
                entry=Global::RS_FPdiv_Queue.erase(entry);
                break;
            }
            entry++;
        }
    }
    executeFPDIV();




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
    return true;
}
//每次cycle都要执行executeINT
void Execute::executeINT(){
    //Global::INT_pipeline所有的指令latency-1,因为INT latency=1,所以其实每次INT_pipeline里就只有一条指令
    //INT_pipeline_entry遍历INT_pipeline里所有的指令
    for(auto entry=Global::INT_pipeline.begin();entry!=Global::INT_pipeline.end();){
        if(Global::INT_pipeline.empty()) break;//没有指令什么都不用执行
        entry->remaining_latency--;
        if(entry->remaining_latency==0){
            int result=0;
            //运算完成
            //执行指令
            if(entry->rs_entry.opcode==add)result=entry->rs_entry.Vj+entry->rs_entry.Vk;
            else if(entry->rs_entry.opcode==addi)result=entry->rs_entry.Vj+entry->rs_entry.A.value();
            else if(entry->rs_entry.opcode==slt)result=entry->rs_entry.Vj<entry->rs_entry.Vk;
            else{
                throw runtime_error("opcode is not INT");
            }
            entry->rs_entry.result=result;
            //写进completeRSqueue,代表计算完成的指令
            insertCompletedEntry(entry->rs_entry);
            //ROB和CDB在write back阶段更新
        }
        entry++;
    }
}


void Execute::executeLOAD(){
    //Global::LOAD_pipeline所有的指令latency-1,因为LOAD latency=1,所以其实每次LOAD_pipeline里就只有一条指令
    //LOAD_pipeline_entry遍历LOAD_pipeline里所有的指令
    for(auto entry=Global::LOAD_pipeline.begin();entry!=Global::LOAD_pipeline.end();){
        if(Global::LOAD_pipeline.empty()) break;//没有指令什么都不用执行
        entry->remaining_latency--;
        if(entry->remaining_latency==0){
            int memory_address=0;
            //运算完成
            //执行指令
            //Vj应该是物理寄存器存的值,能送到execute的都ready了
            if(entry->rs_entry.opcode==fld)memory_address=entry->rs_entry.Vj+entry->rs_entry.A.value();
            else{
                throw runtime_error("opcode is not LOAD");
            }
            entry->rs_entry.result=memory_address;
            //写进completeRSqueue,代表计算完成的指令
            insertCompletedEntry(entry->rs_entry);
            //ROB和CDB在write back阶段更新
        }
        entry++;
    }
}

void Execute::executeSTORE(){
    for(auto entry=Global::STORE_pipeline.begin();entry!=Global::STORE_pipeline.end();){
        if(Global::STORE_pipeline.empty()) break;//没有指令什么都不用执行
        entry->remaining_latency--;
        if(entry->remaining_latency==0){
            int memory_address=0;
            //运算完成
            //执行指令
            //Vj应该是物理寄存器存的值,能送到execute的都ready了,fsd计算地址，Vk是地址
            if(entry->rs_entry.opcode==fsd)memory_address=entry->rs_entry.Vk+entry->rs_entry.A.value();
            else{
                throw runtime_error("opcode is not STORE");
            }
            entry->rs_entry.result=memory_address;//Vj也一起传进去了
            //写进completeRSqueue,代表计算完成的指令
            insertCompletedEntry(entry->rs_entry);
            //ROB和CDB在write back阶段更新
        }
        entry++;
    }
}
void Execute::executeFPADD(){
    for(auto entry=Global::FPadd_pipeline.begin();entry!=Global::FPadd_pipeline.end();){
        if(Global::FPadd_pipeline.empty()) break;//没有指令什么都不用执行
        entry->remaining_latency--;
        if(entry->remaining_latency==0){
            int result=0;
            //运算完成
            //执行指令
            if(entry->rs_entry.opcode==fadd)result=entry->rs_entry.Vj+entry->rs_entry.Vk;
            else if(entry->rs_entry.opcode==fsub)result=entry->rs_entry.Vj-entry->rs_entry.Vk;
            else{
                throw runtime_error("opcode is not FPADD");
            }
            entry->rs_entry.result=result;
            //写进completeRSqueue,代表计算完成的指令
            insertCompletedEntry(entry->rs_entry);
            //ROB和CDB在write back阶段更新
        }
        entry++;
    }
}
void Execute::executeFPMULT(){
    for(auto entry=Global::FPmult_pipeline.begin();entry!=Global::FPmult_pipeline.end();){
        if(Global::FPmult_pipeline.empty()) break;//没有指令什么都不用执行
        entry->remaining_latency--;
        if(entry->remaining_latency==0){
            int result=0;
            //运算完成
            //执行指令
            if(entry->rs_entry.opcode==fmul)result=entry->rs_entry.Vj*entry->rs_entry.Vk;
            else{
                throw runtime_error("opcode is not FPMULT");
            }
            entry->rs_entry.result=result;
            //写进completeRSqueue,代表计算完成的指令
            insertCompletedEntry(entry->rs_entry);
            //ROB和CDB在write back阶段更新
        }
        entry++;
    }
}


void Execute::executeFPDIV(){
    for(auto entry=Global::FPdiv_pipeline.begin();entry!=Global::FPdiv_pipeline.end();){
        if(Global::FPdiv_pipeline.empty()) break;//没有指令什么都不用执行
        entry->remaining_latency--;
        if(entry->remaining_latency==0){
            int result=0;
            //运算完成
            //执行指令
            if(entry->rs_entry.opcode==fdiv)result=entry->rs_entry.Vj/entry->rs_entry.Vk;
            else{
                throw runtime_error("opcode is not FPDIV");
            }
            entry->rs_entry.result=result;
            //写进completeRSqueue,代表计算完成的指令
            insertCompletedEntry(entry->rs_entry);
            //ROB和CDB在write back阶段更新
        }
        entry++;
    }
}

bool Execute::executeBU(int earliest_ID_in_Queue){
    //还是是begin开始，按照之前读指的顺序，begin是最早的。如果begin最小且true,那earliest_ID_in_Queue就变成RS BU中ID_in_Queue第二小的
    for(auto entry=Global::BU_pipeline.begin();entry!=Global::BU_pipeline.end();){
        if(Global::BU_pipeline.empty()) break;//没有指令什么都不用执行
        entry->remaining_latency--;
        if(entry->remaining_latency==0){
            int result=0;
            //运算完成
            //执行指令
            if(entry->rs_entry.opcode==bne)result=entry->rs_entry.Vj==entry->rs_entry.Vk;
            else{
                throw runtime_error("opcode is not BU");
            }
            entry->rs_entry.result=result;
            //在BTB中得到之前的分支预测结果
            bool predictTaken=Global::btb.getPrediction(entry->rs_entry.ID_in_Queue);
            bool predictTrueFalse=(predictTaken==result);
            //如果ID最大且预测正确，则earliest_ID_in_Queue变成RS BU中ID_in_Queue第二小的
            if(earliest_ID_in_Queue==entry->rs_entry.ID_in_Queue && predictTrueFalse){
                earliest_ID_in_Queue=getEarliestIDIn_RS_BU_Queue();
                //更新预测器状态
                Global::btb.update(entry->rs_entry.ID_in_Queue,result);
                //写进completeRSqueue,代表计算完成的指令
                insertCompletedEntry(entry->rs_entry);
            }else if(earliest_ID_in_Queue==entry->rs_entry.ID_in_Queue && !predictTrueFalse){
                //立刻回滚，整个execute pipieline结束运行
                Global::historySnapshot.predictionTrueFalseRecover(Global::instructionQueue[entry->rs_entry.ID_in_Queue],predictTaken);
                return false;
            }else{//不能在Execute直接回滚，只有传给ROB决定了,ROB还要判断回滚
                entry->rs_entry.predictTrueFalse=predictTrueFalse;
                Global::btb.update(entry->rs_entry.ID_in_Queue,result);
                insertCompletedEntry(entry->rs_entry);
            }
            //ROB和CDB在write back阶段更新
        }
        entry++;
    }
    return true;
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
int Execute::getEarliestIDIn_RS_BU_Queue() {
    if (Global::RS_BU_Queue.empty()) {
        return -1; // 或者你可以选择抛出异常 / 返回 std::optional<int>
    }

    auto it = std::min_element(
        Global::RS_BU_Queue.begin(), Global::RS_BU_Queue.end(),
        [](const ReservationStationEntry& a, const ReservationStationEntry& b) {
            return a.ID_in_Queue < b.ID_in_Queue;
        }
    );

    return it->ID_in_Queue;
}