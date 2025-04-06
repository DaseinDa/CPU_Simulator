#pragma once
#include <string>
#include <vector>
#include "instruction.h"
#include "instruction_type.h"
#include "archiRegister.h"
using namespace std;
struct ReservationStationEntry {
    bool busy=0;             // 是否被占用
    InstructionType opcode;        // 操作类型，如 "ADD", "MUL", "LOAD" 等

    int Vj = 0;            // 操作数1的值（如果已就绪）
    int Vk = 0;            // 操作数2的值（如果已就绪）
    
    string Qj="";        // 操作数1依赖的ROB/tag（如果未就绪）初始化为未就绪
    string Qk="";        // 操作数2依赖的ROB/tag（如果未就绪）

    string dest="";      // 目的寄存器/ROB条目标识符（写结果去哪里）

    int latency = 0;       // 执行所需时钟周期（用于多周期指令）
    int issueCycle = -1;   // 指令被调度的周期（用于统计和调试）
    int dispatchCycle = -1;// 指令进入RS的周期

    int ID_in_Queue;    // 可选：指向原始指令对象（方便调试/追踪）

void reset() {
        busy = false;
        op = "";
        Vj = Vk = 0;
        Qj = Qk = "";
        dest = "";
        instr = nullptr;
    }
};

bool hasFreeEntry(const std::vector<ReservationStationEntry>& rs) {
    for (const auto& entry : rs) {
        if (!entry.busy) {
            return true;  // 找到一个空槽
        }
    }
    return false;  // 没有空位
}
int getFreeEntryIndex(const std::vector<ReservationStationEntry>& rs) {
    for (size_t i = 0; i < rs.size(); ++i) {
        if (!rs[i].busy) {
            return static_cast<int>(i);
        }
    }
    return -1;  // -1 表示没有空位
}

bool isReadyToIssue(const ReservationStationEntry& entry) {
    // 一些单操作数指令（如 fld，只有 Qj）
    if (entry.op == "fld" || entry.op == "jal" || entry.op == "addi" || entry.op == "load") {
        return entry.Qj.empty();  // 只要第一个源就绪
    }

    // Store 类型（fsd）：地址（Qj）和要写入的值（Qk）都要 ready
    if (entry.op == "fsd" || entry.op == "sw" || entry.op == "store") {
        return entry.Qj.empty() && entry.Qk.empty();  // 都需要就绪
    }

    // 双操作数指令（如 add、mul、fadd、sub）
    return entry.Qj.empty() && entry.Qk.empty();  // 正常通用型指令
}

void issueToExecute(std::vector<ReservationStationEntry>& rs, FunctionalUnit& alu) {
    for (auto& entry : rs) {
        if (entry.busy && entry.Qj.empty() && entry.Qk.empty()) {
            if (alu.isAvailable()) {
                alu.startExecution(entry);
                entry.busy = false; // 或者设为 executing 状态
                rs[i].reset();
                break;  // 只发射一个（或者可以并发发射多个）
            }
        }
    }
}
bool insertToRS(const Instruction& instr, vector<ReservationStationEntry>& rs_pool,ReorderBuffer& rob) {
    
    // Step 1: 找一个空位
    int slot = -1;
    for (size_t i = 0; i < rs_pool.size(); ++i) {
        if (!rs_pool[i].busy) {
            slot = i;
            break;
        }
    }
    if (slot == -1) return false; // 保留站已满，decode stall

    auto& entry = rs_pool[slot];
    entry.busy = true;
    entry.op = instr.opcode;

    // Step 2: 设置 Vj/Qj
    if (Global::renaming_worker.registerRenamingMapping.count(instr.operands[1])) {
        entry.Qj = Global::renaming_worker.registerRenamingMapping[instr.rs1];  // 有依赖
    } else if (Global::architectureRegisterFile[instr.operands[1]].isReady) {
        entry.Vj = Global::architectureRegisterFile[instr.operands[1]].value;
        entry.Qj = "";  // 就绪
    } else {
        entry.Qj = "UNKNOWN";  // 没有映射又没 ready？debug case
    }

    // Step 3: 设置 Vk/Qk（双操作数指令才需要）
    if (instr.opcode == "add" || instr.opcode == "sub" || instr.opcode == "mul" || instr.opcode == "fadd") {
        if (Global::renaming_worker.registerRenamingMapping.count(instr.operands[2])) {
            entry.Qk = Global::renaming_worker.registerRenamingMapping[instr.operands[2]];
        } else if (Global::architectureRegisterFile[instr.operands[2]].isReady) {
            entry.Vk = Global::architectureRegisterFile[instr.operands[2]].value;
            entry.Qk = "";
        } else {
            entry.Qk = "UNKNOWN";
        }
    } else {
        // 单操作数指令
        entry.Qk = "";
        entry.Vk = 0;
    }

    // Step 4: 分配 ROB，更新 dest & 重命名表
    std::string rob_tag = rob.allocateEntry(instr.rd);  // 返回 "ROB5"
    entry.dest = rob_tag;
    rat.table[instr.rd] = rob_tag;  // 更新重命名表：逻辑寄存器 -> ROB tag

    return true;
}
