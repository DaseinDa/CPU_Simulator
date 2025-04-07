#pragma once
#include "string.h"
#include <iostream>
#include <vector>
#include <unordered_map>
#include <iomanip>
#include <deque>
#include <optional>
#include <sstream>
#include "unit.h"
#include "instruction_type.h"
#include "unit_type.h"
using namespace std;


// typedef struct reg {
//     string register_name;
//     int value;
// };每条指令的寄存器不同,但是每条指令的寄存器的值是一样的,所以不能在instruction里定义值
class Instruction{
public:
    InstructionType opcode;
    vector<string> operands;
    optional<double> immediate;
    optional<string> label;
    optional<string> target;
    int executionCount=0;
    int instructionNumber=0;
    //当开始fetch时候，记录指令的程序顺序和status
    optional<int> ID_in_Queue;
    optional<int> status_in_Queue;
    string Qj;
    string Qk;
    double Vj=0;
    double Vk=0;
    // Instruction(){};
    Instruction() {};
    Instruction(InstructionType op, const std::vector<std::string>& ops,
                const std::optional<std::string>& lbl = std::nullopt,
                const std::optional<std::string>& tgt = std::nullopt)
        : opcode(op), operands(ops), label(lbl), target(tgt) {
            recognizeImmediate();
        };

    void recognizeImmediate();
    void fetchExecutionCount(){
        executionCount++;
    }
    void executeInstruction(){
        if(opcode == addi){
            cout<<"This is a addi instruction"<<endl;
        }
};
    UnitType getUnitType();
    int getLatency();
    // 打印指令信息
    void print() const {
        std::cout << "Opcode: " << static_cast<int>(opcode) << ", Operands: ";
        for (const auto& op : operands) {
            std::cout << op << " ";
        }
        if(immediate.has_value()){
            std::cout << ", immediate: " << immediate.value();
        }
        if (label.has_value()) {
            std::cout << ", Label: " << label.value();
        }
        if (target.has_value()) {
            std::cout << ", Target: " << target.value();
        }
        std::cout << std::endl;
    }

};

string InstructionTypetoInstr(const InstructionType opcode);
InstructionType toInstructionType(const std::string opcode);
string addSpaceAfterComma(const std::string& line);