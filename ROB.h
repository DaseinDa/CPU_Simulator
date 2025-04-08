#pragma once
#include "common.h"
#include <string>
#include <optional>
using namespace std;
struct ROBEntry {
    int ID_in_Queue;//通过ID_in_Queue在全局维护的InstructionQueue中查看指令Status
    InstructionStatus status;
    optional<string> dest_archi_register;
    optional<string> dest_physical_register;
    optional<double> value;

};//在这个地方ID_in_Queue就是robTail,每条指令在程序顺序产生的时候，就在ROB里有唯一的位置



// store_queue[5] = {
//     rob_id: 10,
//     addr: 0xABCD1234,
//     value: 42.0,
//     addr_ready: true,
//     value_ready: true
// }