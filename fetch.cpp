#include "fetch.h"
Fetch::Fetch(){
        cout<<"Here is the fetch stage, fetch.h file"<<endl;
        };


bool Fetch::fetch(){
    cout<<"--------------------------Fetch-------------------------"<<endl;
    cout<<"fetch dispatching..., nf="<<nf<<endl;
    if(Global::fetchInstructionQueue.size()>=nf){
        return false;//如果已经没有空余bandwidth,什么都不fetch,count as a stall
    }
    int startFetchQueueSize=Global::fetchInstructionQueue.size();
    int capacity=nf-startFetchQueueSize;
    for(int i=0;i<capacity;i++){
        cout<<"i="<<i<<endl;
        if(Global::instructionset.empty()||Global::fetch_pointer >=Global::instructionset.size()){
                cout<<"All instructions have been taken in the pipeline"<<endl;
                return true;
        }else{ 
            fetchInstruction();//for 循环中不能return,否则剩下的bandwidth没有做fetch
        }
    }
    return (Global::instructionset.empty()|| startFetchQueueSize==0);//既没有取完，又存在之前的指令stall了
}
void Fetch::fetchInstruction(){
    Global::instructionset[Global::fetch_pointer].fetchExecutionCount();
    Instruction fetch_intr = Global::instructionset[Global::fetch_pointer];
    fetch_intr.ID_in_Queue=Global::instructionQueue.size();
    Global::fetchInstructionQueue.push_back(fetch_intr);
    fetch_intr.status_in_Queue=InstructionStatus::FETCH;
    Global::instructionQueue.push_back(fetch_intr);
    //如果指令是bne还需要另外写代码判断，但现在先不考虑bne的情况
    if(fetch_intr.opcode==InstructionType::bne){//branch prediction
        string target_str = fetch_intr.target.value();
        int target_position=Global::btb.getTargetPosition(fetch_intr.instructionNumber);
        // //添加历史快照
        if(target_position!=-1){
            Global::fetch_pointer=target_position;
            fetch_intr.bne_taken=true;
        }else{
            Global::fetch_pointer++;// oredict n
            fetch_intr.bne_taken=false;
        }
        Global::historySnapshot.addSnapshot(fetch_intr);
    }else{
        Global::fetch_pointer++;
    }
}
// cout << "Fetch构造函数中 instructionset 地址: " << &(this->instructionset) << endl;
// cout << "instructionset size " << (this->instructionset).size()<< endl;
// cout << "dispatch中 instructionset 地址: " << &(this->instructionset) << endl;