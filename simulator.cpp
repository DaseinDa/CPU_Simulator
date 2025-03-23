#include "simulator.h"
std::unordered_map<std::string, size_t> labelMap; 
void ReadAssemblyFile(char * PathFile){
    string filepath = PathFile;
    string line;
    string currentLabel;
    bool inLabelScope = false;
    std::vector<Instruction> instructions;
    // InstructionType l=addi;
    ifstream ifs;
    ifs.open(filepath);
    if(ifs.is_open()){
        cout<< "Reading File..."<<endl;
        // 读取汇编代码
        size_t index = 0;
        while(getline(ifs, line)){
                cout<<line<<endl;
                line.erase(0, line.find_first_not_of(" \t"));
                line.erase(line.find_last_not_of(" \t") + 1);
                if (line.empty()) continue;
                // 解析标签
                size_t colonPos = line.find(':');
                std::string label = "";
                if (colonPos != std::string::npos) {
                    label = line.substr(0, colonPos);
                    line = line.substr(colonPos + 1);
                    line.erase(0, line.find_first_not_of(" \t"));
                    labelMap[label] = index; // 记录标签对应的指令索引
                }

        // 解析指令
        std::istringstream iss(line);//一定要有空格才能识别,输入文件按照特定格式
        std::string opcode, op1, op2, op3;
        iss >> opcode >> op1 >> op2 >> op3;

        // 去除逗号
        op1.erase(std::remove(op1.begin(), op1.end(), ','), op1.end());
        op2.erase(std::remove(op2.begin(), op2.end(), ','), op2.end());
        if (!op3.empty()) {
            op3.erase(std::remove(op3.begin(), op3.end(), ','), op3.end());
        }

        // 将操作码转换为 InstructionType
        InstructionType type = toInstructionType(opcode);
        optional<string> optLabel = label.empty() ? nullopt : optional<string>(label);

        // 处理跳转目标
        std::optional<std::string> target = std::nullopt;
        if (type == InstructionType::bne) {
            target = op3; // 对于 bne 指令，最后一个操作数是跳转目标
        }

        // 创建 Instruction 对象并添加到 instructions 向量中
        std::vector<std::string> operands = {op1, op2};
        if (type != InstructionType::bne && !op3.empty()) {
            operands.push_back(op3); // 对于非跳转指令，op3 是操作数
        }
        instructions.emplace_back(type, operands, optLabel, target);
        index++;
                // parseAssemblyLine(line,currentLabel,inLabelScope);
        }
    }
    if(DebugMode){
        for (size_t i = 0; i < instructions.size(); ++i) {
        std::cout << "Instruction " << i << ": ";
        instructions[i].print();
    }
    }

    ifs.close();
    }
