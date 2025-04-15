#pragma once
//forwarding其实就是在execute阶段写终点物理寄存器，并设置为ready状态
//fsd 和fld的forwarding需要单独处理
#include "global.h"
#include <string>
using namespace std;

static void writePRF(string prf_id, double result) {
    if(Global::renaming_worker.physicalRegister.count(prf_id)){
        Global::renaming_worker.physicalRegister.at(prf_id).value = result;
        Global::renaming_worker.physicalRegister.at(prf_id).isReady = true;
    }else{
        throw runtime_error("prf_id not found for writePRF");
    }
}

static void checkRSOperandsReady() {
    cout<<"forwarding..."<<endl;
    for (auto& entry : Global::RS_INT_Queue) {
        // 检查 Qj，如果它不为空，看看对应的 PRF 是否 ready
        if (!entry.Qj.empty()) {
            string prf_id = entry.Qj; // 假设 Qj 存的是 物理寄存器编号
            if(Global::renaming_worker.physicalRegister.count(prf_id)) {
                if (Global::renaming_worker.physicalRegister.at(prf_id).isReady) {
                    entry.Vj = Global::renaming_worker.physicalRegister.at(prf_id).value;
                    entry.Qj="";  //标记为已就绪
                }
            }else{
                throw runtime_error("prf_id not found for RS_INT_Queue");
            }
        }
        if (!entry.Qk.empty()) {
            string prf_id = entry.Qk; // 假设 Qj 存的是 物理寄存器编号
            if(Global::renaming_worker.physicalRegister.count(prf_id)) {
                if (Global::renaming_worker.physicalRegister.at(prf_id).isReady) {
                    entry.Vk = Global::renaming_worker.physicalRegister.at(prf_id).value;
                    entry.Qk="";
                }
            }else{
                throw runtime_error("prf_id not found for RS_INT_Queue");
            }
        }
    }
    for (auto& entry : Global::RS_LOAD_Queue) {
        // 检查 Qj，如果它不为空，看看对应的 PRF 是否 ready
        if (!entry.Qj.empty()) {
            string prf_id = entry.Qj; // 假设 Qj 存的是 物理寄存器编号
            if(Global::renaming_worker.physicalRegister.count(prf_id)) {
                if (Global::renaming_worker.physicalRegister.at(prf_id).isReady) {
                    entry.Vj = Global::renaming_worker.physicalRegister.at(prf_id).value;
                    entry.Qj="";  //标记为已就绪
                }
            }else{
                throw runtime_error("prf_id not found for RS_LOAD_Queue");
            }
        }
        if (!entry.Qk.empty()) {
            string prf_id = entry.Qk; // 假设 Qj 存的是 物理寄存器编号
            if(Global::renaming_worker.physicalRegister.count(prf_id)) {
                if (Global::renaming_worker.physicalRegister.at(prf_id).isReady) {
                    entry.Vk = Global::renaming_worker.physicalRegister.at(prf_id).value;
                    entry.Qk="";
                }
            }else{
                throw runtime_error("prf_id not found for RS_LOAD_Queue");
            }
        }
    }
    for (auto& entry : Global::RS_STORE_Queue) {
        // 检查 Qj，如果它不为空，看看对应的 PRF 是否 ready
        if (!entry.Qj.empty()) {
            string prf_id = entry.Qj; // 假设 Qj 存的是 物理寄存器编号
            if(Global::renaming_worker.physicalRegister.count(prf_id)) {
                if (Global::renaming_worker.physicalRegister.at(prf_id).isReady) {
                    entry.Vj = Global::renaming_worker.physicalRegister.at(prf_id).value;
                    entry.Qj="";  //标记为已就绪
                }
            }else{
                throw runtime_error("prf_id not found for RS_STORE_Queue");
            }
        }
        if (!entry.Qk.empty()) {
            string prf_id = entry.Qk; // 假设 Qj 存的是 物理寄存器编号
            if(Global::renaming_worker.physicalRegister.count(prf_id)) {
                if (Global::renaming_worker.physicalRegister.at(prf_id).isReady) {
                    entry.Vk = Global::renaming_worker.physicalRegister.at(prf_id).value;
                    entry.Qk="";
                }
            }else{
                throw runtime_error("prf_id not found for RS_STORE_Queue");
            }
        }
    }
    for (auto& entry : Global::RS_FPadd_Queue) {
        // 检查 Qj，如果它不为空，看看对应的 PRF 是否 ready
        if (!entry.Qj.empty()) {
            string prf_id = entry.Qj; // 假设 Qj 存的是 物理寄存器编号
            if(Global::renaming_worker.physicalRegister.count(prf_id)) {
                if (Global::renaming_worker.physicalRegister.at(prf_id).isReady) {
                    entry.Vj = Global::renaming_worker.physicalRegister.at(prf_id).value;
                    entry.Qj="";  //标记为已就绪
                }
            }else{
                throw runtime_error("prf_id not found for RS_FPadd_Queue");
            }
        }
        if (!entry.Qk.empty()) {
            string prf_id = entry.Qk; // 假设 Qj 存的是 物理寄存器编号
            if(Global::renaming_worker.physicalRegister.count(prf_id)) {
                if (Global::renaming_worker.physicalRegister.at(prf_id).isReady) {
                    entry.Vk = Global::renaming_worker.physicalRegister.at(prf_id).value;
                    entry.Qk="";
                }
            }else{
                throw runtime_error("prf_id not found for RS_FPadd_Queue");
            }
        }
    }
    for (auto& entry : Global::RS_FPmult_Queue) {
        // 检查 Qj，如果它不为空，看看对应的 PRF 是否 ready
        if (!entry.Qj.empty()) {
            string prf_id = entry.Qj; // 假设 Qj 存的是 物理寄存器编号
            if(Global::renaming_worker.physicalRegister.count(prf_id)) {
                if (Global::renaming_worker.physicalRegister.at(prf_id).isReady) {
                    entry.Vj = Global::renaming_worker.physicalRegister.at(prf_id).value;
                    entry.Qj="";  //标记为已就绪
                }
            }else{
                throw runtime_error("prf_id not found for RS_FPmult_Queue");
            }
        }
        if (!entry.Qk.empty()) {
            string prf_id = entry.Qk; // 假设 Qj 存的是 物理寄存器编号
            if(Global::renaming_worker.physicalRegister.count(prf_id)) {
                if (Global::renaming_worker.physicalRegister.at(prf_id).isReady) {
                    entry.Vk = Global::renaming_worker.physicalRegister.at(prf_id).value;
                    entry.Qk="";
                }
            }else{
                throw runtime_error("prf_id not found for RS_FPmult_Queue");
            }
        }
    }
    for (auto& entry : Global::RS_FPdiv_Queue) {
        // 检查 Qj，如果它不为空，看看对应的 PRF 是否 ready
        if (!entry.Qj.empty()) {
            string prf_id = entry.Qj; // 假设 Qj 存的是 物理寄存器编号
            if(Global::renaming_worker.physicalRegister.count(prf_id)) {
                if (Global::renaming_worker.physicalRegister.at(prf_id).isReady) {
                    entry.Vj = Global::renaming_worker.physicalRegister.at(prf_id).value;
                    entry.Qj="";  //标记为已就绪
                }
            }else{
                throw runtime_error("prf_id not found for RS_FPdiv_Queue");
            }
        }
        if (!entry.Qk.empty()) {
            string prf_id = entry.Qk; // 假设 Qj 存的是 物理寄存器编号
            if(Global::renaming_worker.physicalRegister.count(prf_id)) {
                if (Global::renaming_worker.physicalRegister.at(prf_id).isReady) {
                    entry.Vk = Global::renaming_worker.physicalRegister.at(prf_id).value;
                    entry.Qk="";
                }
            }else{
                throw runtime_error("prf_id not found for RS_FPdiv_Queue");
            }
        }
    }
    for (auto& entry : Global::RS_BU_Queue) {
        // 检查 Qj，如果它不为空，看看对应的 PRF 是否 ready
        if (!entry.Qj.empty()) {
            string prf_id = entry.Qj; // 假设 Qj 存的是 物理寄存器编号
            if(Global::renaming_worker.physicalRegister.count(prf_id)) {
                if (Global::renaming_worker.physicalRegister.at(prf_id).isReady) {
                    entry.Vj = Global::renaming_worker.physicalRegister.at(prf_id).value;
                    entry.Qj="";  //标记为已就绪
                }
            }else{
                throw runtime_error("prf_id not found for RS_BU_Queue");
            }
        }
        if (!entry.Qk.empty()) {
            string prf_id = entry.Qk; // 假设 Qj 存的是 物理寄存器编号
            if(Global::renaming_worker.physicalRegister.count(prf_id)) {
                if (Global::renaming_worker.physicalRegister.at(prf_id).isReady) {
                    entry.Vk = Global::renaming_worker.physicalRegister.at(prf_id).value;
                    entry.Qk="";
                }
            }else{
                throw runtime_error("prf_id not found for RS_BU_Queue");
            }
        }
    }
}