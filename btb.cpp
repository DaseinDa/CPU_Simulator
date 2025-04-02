#include "btb.h"
BTB::~BTB() {cout<<"BTB destructor called"<<endl;}
//每次分支预测都要判断btb需要不需要更新，所以update每次都调用
void BTB::update(int instructionNumber,bool taken){
    //获取4-7地址
    string address4_7=getInstructionAddress(instructionNumber);
    BranchPredict newBranchPredict;
    if(btbMap.count(address4_7)){//元素存在
        btbMap[address4_7].update(taken);
        btbList.remove(address4_7);
    }else{//元素不存在
        btbMap[address4_7]=newBranchPredict;
    }
    btbList.push_front(address4_7);
    //检查是否超过容量16
    if(btbList.size()>capacity){
        btbList.pop_back();
        btbMap.erase(btbList.back());
    }
}