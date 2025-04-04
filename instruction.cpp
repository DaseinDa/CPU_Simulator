#include "instruction.h"
InstructionType toInstructionType(const std::string opcode) {
    static const std::unordered_map<std::string, InstructionType> opcodeMap = {
        {"fld", InstructionType::fld},
        {"fsd",  InstructionType::fsd},
        {"add", InstructionType::add},
        {"addi", InstructionType::addi},
        {"slt",  InstructionType::slt},
        {"fadd",  InstructionType::fadd},
        {"fsub",  InstructionType::fsub},
        {"fmul",  InstructionType::fmul},
        {"fdiv",  InstructionType::fdiv},
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
 string InstructionTypetoInstr(const InstructionType opcode) {
        static const std::unordered_map<InstructionType, std::string> opcodeMap = {
            {InstructionType::fld,"fld"},
            {InstructionType::fsd,"fsd"},
            {InstructionType::add,"add"},
            {InstructionType::addi,"addi"},
            {InstructionType::slt,"slt"},
            {InstructionType::fadd,"fadd"},
            {InstructionType::fsub,"fsub"},
            {InstructionType::fmul,"fmul"},
            {InstructionType::fdiv,"fdiv"},
            {InstructionType::bne,"bne"}
        };
    
    auto it = opcodeMap.find(opcode);
    if (it != opcodeMap.end()) {
        return it->second; // 返回对应的枚举值
    }else
    {
        cout<<"Unkown instruction"<<endl;
    }

};
string addSpaceAfterComma(const std::string& line) {
    std::string result;
    for (size_t i = 0; i < line.size(); ++i) {
        result += line[i];
        if (line[i] == ',' && i + 1 < line.size() && line[i + 1] != ' ') {
            result += ' ';  // 在逗号后加空格
        }
    }
    return result;
}
void Instruction::recognizeImmediate(){
    switch(opcode){
        case InstructionType::fld: {
            string& temp=operands.back();
            for (int i = 0; i < temp.size(); ++i) {
                if (temp[i] == '(') {
                    immediate = stoi(temp.substr(0, i));
                    temp = temp.substr(i + 1, temp.size() - i - 2);
                    break;
                }
            }
        }
        case InstructionType::fsd: {
            string& temp=operands.back();
            for (int i = 0; i < temp.size(); ++i) {
                if (temp[i] == '(') {
                    immediate = stoi(temp.substr(0, i));
                    temp = temp.substr(i + 1, temp.size() - i - 2);
                    break;
                }
            }
            break;
        }
        case InstructionType::add: {
            break;
        }
        case InstructionType::addi: {
            immediate =stoi(operands.back());
            operands.pop_back();
        }
        case InstructionType::fadd: {
            break;
        }
        case InstructionType::fsub: {
            break;
        }
        case InstructionType::fmul: {
            break;
        }
        case InstructionType::fdiv: {
            break;
        }
        case InstructionType::bne: {
            break;
        }
        break;
        
    }


}
string getInstructionAddress(int instructionNumber) {
    // Calculate address (each instruction is 4 bytes)
    uint32_t address = instructionNumber * 4;
    
    // Convert to hex string with leading zeros
    stringstream ss;
    ss << "0x" << setfill('0') << setw(5) << hex << address;
    
    return ss.str();
}