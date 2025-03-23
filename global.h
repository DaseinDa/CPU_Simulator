#include "string.h"
#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

extern unsigned int NF;
extern unsigned int NI;
extern unsigned int NW;
extern unsigned int NB;
extern unsigned int NR;
extern bool DebugMode;
enum InstructionType {
    fld,
    fsd,
    add,
    addi,
    slt,
    fadd,
    fsub,
    fmul,
    fdiv,
    bne
};
// typedef struct reg {
//     string register_name;
//     int value;
// };每条指令的寄存器不同,但是每条指令的寄存器的值是一样的,所以不能在instruction里定义值
class Instruction{
public:
    InstructionType opcode;
    vector<string> operands;
    // int immediate;
    optional<string> label;
    optional<string> target;
    // Instruction(){};
    Instruction() {};
    Instruction(InstructionType op, const std::vector<std::string>& ops,
                const std::optional<std::string>& lbl = std::nullopt,
                const std::optional<std::string>& tgt = std::nullopt)
        : opcode(op), operands(ops), label(lbl), target(tgt) {};

    void executeInstruction(){
        if(opcode == addi){
            cout<<"This is a addi instruction"<<endl;
        }
};
    // 打印指令信息
    void print() const {
        std::cout << "Opcode: " << static_cast<int>(opcode) << ", Operands: ";
        for (const auto& op : operands) {
            std::cout << op << " ";
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


InstructionType toInstructionType(const std::string opcode);

// 调转执行示例:
// else if (inst.opcode == "ne") {
//         // bne rs, rt, target
//         if (registers[inst.operands[0]] != registers[inst.operands[1]]) {
//             // 跳转到目标标签
//             pc = labelMap.at(inst.target.value());
//             return; // 跳过 pc++
//         }}

// labelMap[label] = index
// if (colonPos != std::string::npos) {
//     label = line.substr(0, colonPos);
//     line = line.substr(colonPos + 1);
//     line.erase(0, line.find_first_not_of(" \t"));
//     labelMap[label] = index; // 记录标签对应的指令索引
        // } 

//用一个数组来记录label_map,每个label对应指令中的第几行