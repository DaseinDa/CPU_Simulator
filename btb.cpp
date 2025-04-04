#include "btb.h"
#include "global.h"

BTB::~BTB() {cout<<"BTB destructor called"<<endl;}
//Tag is the instruction number
void BTB::update(int instructionNumber,bool taken){//在实际bne execute period 分支预测结果出来后使用
    int key=getBTBposition(instructionNumber);
    auto& [tag,target_position,predictor]=btbMap[key];
    if(tag == instructionNumber){
        //update the predictor
        predictor.update(taken);
    }
}

int BTB::getTargetPosition(int instructionNumber){//在fetch阶段使用,return -1 means not taken,同时做了分支预测判断
    int key=getBTBposition(instructionNumber);
    int target_position=-1;
    if(btbMap.count(key)){//索引存在
        auto [tag,target_position,predictor]=btbMap[key];
        //对比Tag一不一样
        if(tag == instructionNumber){
            BTB_HIT[key]=1;   
            if(predictor.predict()){
                return target_position;
            }else{
                return -1;
            }
        }else{//Tag conflict, 未命中,就默认从weak taken状态开始
                BTB_HIT[key]=0;
                auto target_str=Global::instructions[instructionNumber].target.value();
                if(!Global::instructions[instructionNumber].target.has_value() || Global::labelMap.count(target_str)==0){
                    throw runtime_error("######Target not found in the instruction or target position not found in labelMap");   
                }
                target_position=Global::labelMap[target_str];
                BranchPredict replace_predictor;
                btbMap[key]=make_tuple(instructionNumber,target_position,replace_predictor);
                //Replace with new BTB target at the same entry
                return target_position; 
        }
    }else{//索引不存在,未命中
        BTB_HIT[key]=0;
        //添加索引,and return 1 always for the new entry adding
        auto target_str=Global::instructions[instructionNumber].target.value();
        target_position=Global::labelMap[target_str];
        // predict=BranchPredict predictor;
        BranchPredict predictor;
        // BTB_HIT[key]=True; 
        btbMap[key]=make_tuple(instructionNumber,target_position,predictor);
        return target_position;
    }
}

// void BTB::ifPredictTrueFalse(Instruction bne_instr){
//     //如果预测正确，更新cdb
//     //如果预测错误，恢复历史快照,包括btb, 寄存器重命名mappingtable, freelist，ROB， pipeline队列比如fetch queue, issue queue, execute queue, memory queue, write back queue
//     //如果预测错误instructionset也要恢复快照，因为里面记录了executionCount,需要回滚到快照才能继续正确积累。不然fetchInstructionQueue一直是1
//    //依然要更新cdb,更新bne的执行结果到cdb
// }