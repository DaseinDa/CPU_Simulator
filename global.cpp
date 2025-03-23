#include "global.h"
unsigned int NF=4;
bool DebugMode=true;
InstructionType toInstructionType(const std::string opcode) {
    static const std::unordered_map<std::string, InstructionType> opcodeMap = {
        {"addi", InstructionType::addi},
        {"fld",  InstructionType::fld},
        {"fmul", InstructionType::fmul},
        {"fadd", InstructionType::fadd},
        {"fsd",  InstructionType::fsd},
        {"bne",  InstructionType::bne}
    };

    auto it = opcodeMap.find(opcode);
    if (it != opcodeMap.end()) {
        return it->second; // 返回对应的枚举值
    }else
    {
        cout<<"Unkown instruction"<<endl;
    }
    
};
