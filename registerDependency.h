#pragma once
#include <optional>
#include <vector>
#include <string>
#include "global.h"
#include "dependency.h"
using namespace std;

static void updateDependency(Instruction decode_instruction){
    //检查rs寄存器和之前decode的指令是否存在最近依赖关系
    //不能在decodeInstruction里搜索，虽然它已经重命名了，搜索等待中的物理寄存器更方便。但是可能缺失一些已经发送到issueQueue或者executeQueue不在decode里的指令，而这时物理寄存器还在使用中
    //search in instructionQueue,find the depent instructions, reflect to the current physical register according to the mapping
    // search from end()-1 to begin(),find the first instruction's rd that dependent to current rs
    //根据decode_instruction的opcode判断是搜索几个源操作数
    int rs_ID_in_Queue=decode_instruction.ID_in_Queue.value();
    for(int i=1;i<decode_instruction.operands.size();i++){
        //从后往前搜索，找到最新依赖关系
        string rs=decode_instruction.operands[i];
        for(int j=Global::instructionQueue.size()-1;j>=0;j--){
            if(Global::instructionQueue[j].operands[0]==rs){
                //找到最新依赖关系，更新当前指令的物理寄存器, return j as ID in Queue
                string rs_physical=Global::renaming_worker.getPhysicalRegID(rs);
                Dependency dependency_rd;
                dependency_rd.ID_in_Queue=j;
                dependency_rd.rd=decode_instruction.operands[0];
                //可能commit了不存在寄存器映射
                if(Global::instructionQueue[j].status_in_Queue==InstructionStatus::COMMIT){
                    //commit了，物理寄存器已经释放，读archi寄存器的值
                }else{
                dependency_rd.physical_register=Global::renaming_worker.getPhysicalRegID(dependency_rd.rd);
                }
                tuple<string,string,Dependency> tuple=make_tuple(rs,rs_physical,dependency_rd);
                DependencyList dependency_list;
                dependency_list.ID_in_Queue=rs_ID_in_Queue;
                dependency_list.rs_dependency.push_back(tuple);
                Global::dependency_map[rs_ID_in_Queue]=dependency_list;
                break;
            }
        }
    }
}