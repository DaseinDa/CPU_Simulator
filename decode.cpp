#include "decode.h"


bool Decode::dispatch(){
    if(Global::decodeInstructionQueue.size()>=ni){
        return false;//没有任何capacity可以dispatch不做任何操作
    }
    int startDecodeQueueSize=Global::decodeInstructionQueue.size();
    int capacity=ni-startDecodeQueueSize;
    for(int i=0;i<capacity;i++){
        if(Global::fetchInstructionQueue.empty()){
            return true;//if not instruction awaiting to decode, no stall
        }
        //开始decode
        //renaming
        Instruction decode_instruction=Global::fetchInstructionQueue.front();
     
        if(Global::renaming_worker.instructionRegisterRenaming(decode_instruction)){
            //decode_instruction是被直接修改的
            Global::fetchInstructionQueue.pop_front();
            Global::decodeInstructionQueue.push_back(decode_instruction);
            //检查rs寄存器和之前decode的指令是否存在最近依赖关系
        }else{
            Global::renameStall++;
            return false;//decode stall, physical register not available可以停止执行了
        }
    }
    return (Global::fetchInstructionQueue.empty() || startDecodeQueueSize==0);
}
