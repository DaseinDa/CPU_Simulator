#include "btb.h"
BTB::~BTB() {cout<<"BTB destructor called"<<endl;}

void BTB::update(int instructionNumber,bool taken){//在实际bne execute period 分支预测结果出来后使用
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