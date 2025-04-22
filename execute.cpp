#include "execute.h"

bool Execute::execute(){
    cout<<"--------------------------Execute-------------------------"<<endl;
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
    if(Global::RS_BU_Queue.empty()) cout<<"No instruction in BU Reservation Station"<<endl;
    if(!Global::RS_BU_Queue.empty()){
        for(issue=0;issue<UNIT_BU_NUMBER;issue++){
            //从ready的指令中找到最早的指令优先执行
            if(Global::RS_BU_Queue.empty()) {cout<<"No instruction in BU Reservation Station"<<endl; break;}
            auto it = std::min_element(
                Global::RS_BU_Queue.begin(), Global::RS_BU_Queue.end(),
                [](const ReservationStationEntry& a, const ReservationStationEntry& b) {
                    // 两者都 ready 才比较 ID
                    if (a.Qj.empty() && a.Qk.empty() && b.Qj.empty() && b.Qk.empty()) {
                        return a.ID_in_Queue < b.ID_in_Queue;
                    }
                    // a ready, b not ready -> a 优先
                    if (a.Qj.empty() && a.Qk.empty()) return true;
                    // b ready, a not ready -> b 优先
                    if (b.Qj.empty() && b.Qk.empty()) return false;
                    // 都不 ready，随便
                    return false;
                }
            );
if (it != Global::RS_BU_Queue.end() && it->Qj.empty() && it->Qk.empty()) {
            BU_pipeline_entry.rs_entry=*it;
            BU_pipeline_entry.remaining_latency=it->latency;
            Global::BU_pipeline.push_back(BU_pipeline_entry);
            Global::instructionQueue[it->ID_in_Queue].status_in_Queue=InstructionStatus::EXECUTE;
            issue++;
            //先进pipeline才能删除，不然就读不到了，从RS中删除
            it=Global::RS_BU_Queue.erase(it);
            break;
            cout<<"The ready earliest ID_in_Queue in RS_BU_Queue is: "<<it->ID_in_Queue<<endl;
}
        }
    }
    cout<<"The size of RS_BU_Queue is: "<<Global::RS_BU_Queue.size()<<endl;
    bool predictFalsestall=executeBU(earliest_ID_in_Queue);
    if(!predictFalsestall) return false;



    PipelineStage INT_pipeline_entry;
    issue=0;
    if(!Global::RS_INT_Queue.empty()){
        cout<<"execute!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
        for (auto entry=Global::RS_INT_Queue.begin();entry!=Global::RS_INT_Queue.end() && issue<UNIT_INT_NUMBER;) {
            if(Global::RS_INT_Queue.empty())break;
            if(entry->Qj.empty()&&entry->Qk.empty()){
                Global::instructionQueue[entry->ID_in_Queue].status_in_Queue=InstructionStatus::EXECUTE;
                //发射进INT_pipeline
                INT_pipeline_entry.rs_entry=*entry;
                INT_pipeline_entry.remaining_latency=entry->latency;
                Global::INT_pipeline.push_back(INT_pipeline_entry);
                Global::instructionQueue[entry->ID_in_Queue].status_in_Queue=InstructionStatus::EXECUTE;
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
            if(Global::RS_LOAD_Queue.empty()){break;}
            if(entry->Qj.empty()&&entry->Qk.empty()){
                Global::instructionQueue[entry->ID_in_Queue].status_in_Queue=InstructionStatus::EXECUTE;
                //发射进LOAD_pipeline
                LOAD_pipeline_entry.rs_entry=*entry;
                LOAD_pipeline_entry.remaining_latency=entry->latency;
                Global::LOAD_pipeline.push_back(LOAD_pipeline_entry);
                Global::instructionQueue[entry->ID_in_Queue].status_in_Queue=InstructionStatus::EXECUTE;
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

                Global::instructionQueue[entry->ID_in_Queue].status_in_Queue=InstructionStatus::EXECUTE;
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

                Global::instructionQueue[entry->ID_in_Queue].status_in_Queue=InstructionStatus::EXECUTE;
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
                Global::instructionQueue[entry->ID_in_Queue].status_in_Queue=InstructionStatus::EXECUTE;
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
                Global::instructionQueue[entry->ID_in_Queue].status_in_Queue=InstructionStatus::EXECUTE;
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
            //  Forwarding 原子操作：写入物理寄存器（PRF）并设置为ready
            string prf_id = entry->rs_entry.destPhysicalRegister;
            writePRF(prf_id, double(result));
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
            int allready=LoadStore::allFSDReadyAndForwardValue(entry->rs_entry.destROB);
            //如果前面不存在fsd指令，则写进LoadQueue,代表计算完成的指令且forwarding
            if(!LoadStore::existsFSDInROB(entry->rs_entry.ID_in_Queue)){
                LoadResult loadResult;
                double value=Global::memory_value[memory_address];
                loadResult.rob_id=entry->rs_entry.destROB;
                loadResult.prf_id=entry->rs_entry.destPhysicalRegister;
                loadResult.value=value;
                loadResult.ID_in_Queue=entry->rs_entry.ID_in_Queue;
                LoadStore::insertLoadQueue(loadResult);
                writePRF(entry->rs_entry.destPhysicalRegister, value);//直接forwarding
            }else if(allready){
                //判断是否所有的fsd都ready了，ready的fsd存储在StoreQueue中,检查在不在StoreQueue
                //判断memory_address是否一样，如果一样直接forwarding
                //如果不存在memory_address的，则forward当前memory list值
               optional<double> value=LoadStore::findLatestStoreBeforeWithAddress(memory_address,entry->rs_entry.destROB);
               if(value.has_value()){
                    LoadResult loadResult;
                    loadResult.rob_id=entry->rs_entry.destROB;
                    loadResult.prf_id=entry->rs_entry.destPhysicalRegister;
                    loadResult.value=value.value();
                    LoadStore::insertLoadQueue(loadResult);
                    writePRF(entry->rs_entry.destPhysicalRegister, value.value());//直接forwarding
                }else{//说明全部ready,但是不存在memory_address一样的fsd，和没有fsd一样forward
                    LoadResult loadResult;
                    double value=Global::memory_value[memory_address];
                    loadResult.rob_id=entry->rs_entry.destROB;
                    loadResult.prf_id=entry->rs_entry.destPhysicalRegister;
                    loadResult.value=value;
                    loadResult.ID_in_Queue=entry->rs_entry.ID_in_Queue;
                    LoadStore::insertLoadQueue(loadResult);
                    writePRF(entry->rs_entry.destPhysicalRegister, value);//直接forwarding
                }
            }else if(!allready){
                //说明存在fsd，但是没有ready，则加入PendingLoad,无法执行forwarding
                PendingLoad pendingLoad;
                pendingLoad.rob_id=entry->rs_entry.destROB;
                pendingLoad.prf_id=entry->rs_entry.destPhysicalRegister;
                pendingLoad.address=memory_address;
                pendingLoad.ID_in_Queue=entry->rs_entry.ID_in_Queue;
                LoadStore::insertPendingLoadQueue(pendingLoad);
            }else{
                throw runtime_error("unexpected fsd fld relationship");
            }
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
            //写进completeRSqueue,代表计算完成的指令
            ReadyStore readyStore;
            readyStore.rob_id=entry->rs_entry.destROB;
            readyStore.prf_id=entry->rs_entry.destPhysicalRegister;
            readyStore.address=memory_address;
            readyStore.value=entry->rs_entry.Vj;
            readyStore.ID_in_Queue=entry->rs_entry.ID_in_Queue;
            LoadStore::insertStoreQueue(readyStore);
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
            //  Forwarding 原子操作：写入物理寄存器（PRF）并设置为ready
            string prf_id = entry->rs_entry.destPhysicalRegister;
            writePRF(prf_id, double(result));
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
            double result=0;
            //运算完成
            //执行指令
            if(entry->rs_entry.opcode==fmul)result=entry->rs_entry.Vj*entry->rs_entry.Vk;
            else{
                throw runtime_error("opcode is not FPMULT");
            }
            entry->rs_entry.result=result;
            //写进completeRSqueue,代表计算完成的指令
            insertCompletedEntry(entry->rs_entry);
            //从pipeline中删除
            string prf_id = entry->rs_entry.destPhysicalRegister;
            writePRF(prf_id, double(result));
            entry = Global::FPmult_pipeline.erase(entry); // 最后再 erase

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
            //从pipeline中删除
            //  Forwarding 原子操作：写入物理寄存器（PRF）并设置为ready
            string prf_id = entry->rs_entry.destPhysicalRegister;
            writePRF(prf_id, double(result));
            entry=Global::FPdiv_pipeline.erase(entry);
            //ROB和CDB在write back阶段更新
        }
        entry++;
    }
}

bool Execute::executeBU(int earliest_ID_in_Queue){
    //It still starts with begin. According to the order of reading the pointer before, begin is the earliest. If begin is the smallest and true, then earliest_ID_in_Queue becomes the second smallest ID_in_Queue in RS BU
    for(auto entry=Global::BU_pipeline.begin();entry!=Global::BU_pipeline.end();){
        if(Global::BU_pipeline.empty()) {cout<<"No instruction in BU_pipeline"<<endl; break;}//没有指令什么都不用执行
        entry->remaining_latency--;
        if(entry->remaining_latency==0){
            bool result=false;
            //Operation completed
            //Execute instruction
            cout<<"R1 is: "<<entry->rs_entry.Vj<<endl;
            if(entry->rs_entry.opcode==bne)result=(entry->rs_entry.Vj!=entry->rs_entry.Vk);
            else{
                throw runtime_error("opcode is not BU");
            }
            entry->rs_entry.result=result;
            //to get the snapshot
            Snapshot* snapshot=Global::historySnapshot.findMatchingSnapshot(entry->rs_entry.ID_in_Queue);
            //Get the previous branch prediction result in BTB
            bool predictTaken=snapshot->bne_instruction.bne_taken.value();
            bool predictTrueFalse=(predictTaken==result);
            //If the ID is the largest and the prediction is correct, the earliest_ID_in_Queue becomes the second smallest ID_in_Queue in the RS BU
            //The prediction value does not only come from the branch predictor. There is also the value set after rollback
            if(earliest_ID_in_Queue==entry->rs_entry.ID_in_Queue && predictTrueFalse){
                earliest_ID_in_Queue=getEarliestIDIn_RS_BU_Queue();
                //Update predictor status
                entry->rs_entry.result = result;
                Global::btb.update(entry->rs_entry.ID_in_Queue,result);
                //Write into completeRSqueue, indicating the instruction of the completed calculation
                entry->rs_entry.predictTrueFalse=predictTrueFalse;
                // entry->rs_entry.destPhysicalRegister=entry->rs_entry.destPhysicalRegister;
                insertCompletedEntry(entry->rs_entry);
                entry = Global::BU_pipeline.erase(entry);
            }else if(earliest_ID_in_Queue==entry->rs_entry.ID_in_Queue && !predictTrueFalse){
                //Roll back immediately, and the entire execute pipeline ends.
                entry->rs_entry.result = result;
                entry->rs_entry.predictTrueFalse=predictTrueFalse;
                Global::btb.update(entry->rs_entry.ID_in_Queue,result);
                // entry->rs_entry.destPhysicalRegister=entry->rs_entry.destPhysicalRegister;
                // insertCompletedEntry(entry->rs_entry);
                // entry = Global::BU_pipeline.erase(entry);
                cout<<"historySnapshot.predictionTrueFalseRecover"<<endl;
                Global::historySnapshot.predictionTrueFalseRecover(Global::instructionQueue[entry->rs_entry.ID_in_Queue],predictTaken);
                return false;
            }else{//You cannot roll back directly in Execute. You have to pass it to ROB for decision. ROB also needs to make a decision on rollback.
                entry->rs_entry.result = result;
                entry->rs_entry.predictTrueFalse=predictTrueFalse;
                entry->rs_entry.destPhysicalRegister=entry->rs_entry.destPhysicalRegister;
                Global::btb.update(entry->rs_entry.ID_in_Queue,result);
                insertBUEntry(entry->rs_entry);
                entry = Global::BU_pipeline.erase(entry);
            }
            //ROB and CDB are updated during the write back phase
        }
        cout<<"The size of BU_pipeline is: "<<Global::BU_pipeline.size()<<endl;
        cout<<"The size of BU_Queue is: "<<Global::BUQueue.size()<<endl;
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
    cout<< ".........................................insertCompletedEntry inserting ID_in_Queue: "<<entry.ID_in_Queue<<endl;
    Global::completeRSQueue.insert(it, entry);  // 插入到正确位置
}

void Execute::insertBUEntry(const ReservationStationEntry& entry) {
    auto it = std::lower_bound(
        Global::BUQueue.begin(),
        Global::BUQueue.end(),
        entry,
        [](const ReservationStationEntry& a, const ReservationStationEntry& b) {
            return a.ID_in_Queue < b.ID_in_Queue;  // 升序排序标准
        }
    );

    Global::BUQueue.insert(it, entry);  // 插入到正确位置
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