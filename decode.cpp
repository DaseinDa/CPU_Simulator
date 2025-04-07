#include "decode.h"
#include "registerDependency.h"
Decode::Decode() {
    // Constructor implementation - initialize any necessary state
}

Decode::~Decode() {
    // Destructor implementation - clean up any resources
}
bool Decode::dispatch(){
    if(Global::decodeInstructionQueue.size()>=ni){
        return false;//没有任何capacity可以dispatch不做任何操作
    }
    int startDecodeQueueSize=Global::decodeInstructionQueue.size();
    int capacity=ni-startDecodeQueueSize;
    cout<<"decodecapacity:"<<capacity<<endl;
    for(int i=0;i<capacity;i++){
        if(Global::fetchInstructionQueue.empty()){
            return true;//if not instruction awaiting to decode, no stall
        }
        //开始decode
        Instruction decode_instruction=Global::fetchInstructionQueue.front();//already with ID in Queue
        // I should read Qj,Qk,Vj,Vk from current RAT firstly before rename
        Global::renaming_worker.QjQkVjVk(decode_instruction);
        //输出decode_instruction
        decode_instruction.print();
        // cout<<"decode_instruction:"<<decode_instruction.instruction<<endl;
        cout<<"Qj:"<<decode_instruction.Qj<<" Qk:"<<decode_instruction.Qk<<" Vj:"<<decode_instruction.Vj<<" Vk:"<<decode_instruction.Vk<<endl;
        //renaming
        // updateDependency(decode_instruction);
        if(Global::renaming_worker.instructionRegisterRenaming(decode_instruction)){
            //decode_instruction是被直接修改的
            Global::fetchInstructionQueue.pop_front();
            //instructionQueue的指令更新状态为decode
            Global::instructionQueue[decode_instruction.ID_in_Queue.value()].status_in_Queue=InstructionStatus::DECODE;
            Global::decodeInstructionQueue.push_back(decode_instruction);
            //检查是否存在依赖关系并更新依赖关系表格,根据依赖关系表格检查physical register是否ready
            // updateDependency(decode_instruction);//这样写不对
            //应该直接在RAT中更新查找Qj,Qk
        }else{
            Global::renameStall++;
            return false;//decode stall, physical register not available可以停止执行了
        }
    }
    return (Global::fetchInstructionQueue.empty() || startDecodeQueueSize==0);
}
