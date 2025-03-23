#include "utils.h"
// // 函数：检查是否为支持的指令

// // 函数：检查是否为支持的指令
// bool isSupportedInstruction(const std::string& instruction) {
//     for (const auto& inst : INSTRUCTIONS) {
//         if (inst == instruction) {
//             return true;
//         }
//     }
//     return false;
// }

// // 函数：解析内存地址格式（如 200(R0)）
// bool parseMemoryAddress(const std::string& operand, int& offset, std::string& baseRegister) {
//     std::regex memoryRegex(R"((\d+)\(([^)]+)\))"); // 匹配 200(R0) 格式
//     std::smatch match;

//     if (std::regex_match(operand, match, memoryRegex)) {
//         offset = std::stoi(match[1]);      // 提取偏移量
//         baseRegister = match[2];           // 提取基地址寄存器
//         return true;
//     }
//     return false;
// }

// // 函数：解析汇编代码行
// void parseAssemblyLine(const std::string& line, std::string& currentLabel, bool& inLabelScope) {
//     // 正则表达式匹配标签和指令
//     std::regex labelRegex(R"((\w+:)\s*(.*))"); // 匹配标签（如 "loop:")
//     std::regex instructionRegex(
//         R"((\w+:)?\s*(\b\w+\b)\s+([^,]+),\s*([^,]+)(?:,\s*([^,]+))?)"
//     );
//     std::smatch match;

//     // 检查是否为标签
//     if (std::regex_match(line, match, labelRegex)) {
//         currentLabel = match[1]; // 提取标签（如 "loop:"）
//         inLabelScope = true;     // 进入标签范围
//         std::cout << "Label: " << currentLabel << std::endl;

//         // 如果标签后有指令，继续解析
//         std::string remainingLine = match[2];
//         if (!remainingLine.empty()) {
//             parseAssemblyLine(remainingLine, currentLabel, inLabelScope);
//         }
//         return;
//     }

//     // 解析指令
//     if (std::regex_match(line, match, instructionRegex)) {
//         std::string instruction = match[2]; // 指令（如 "addi", "fld"）
//         std::string operand1 = match[3];   // 第一个操作数
//         std::string operand2 = match[4];   // 第二个操作数
//         std::string operand3 = match[5];   // 第三个操作数（如果有）

//         // 检查是否为支持的指令
//         if (!isSupportedInstruction(instruction)) {
//             std::cout << "Unsupported instruction: " << instruction << std::endl;
//             return;
//         }

//         // 输出当前标签范围状态
//         if (inLabelScope) {
//             std::cout << "Under label: " << currentLabel << std::endl;
//         } else {
//             std::cout << "No label scope" << std::endl;
//         }

//         // 输出指令
//         std::cout << "Instruction: " << instruction << std::endl;

//         // 输出操作数
//         std::cout << "Operand 1: " << operand1 << std::endl;
//         std::cout << "Operand 2: " << operand2 << std::endl;
//         if (!operand3.empty()) {
//             std::cout << "Operand 3: " << operand3 << std::endl;
//         }

//         // 解析操作数
//         std::regex registerRegex(R"((R\d+|F\d+|\$0))"); // 匹配寄存器（如 R1, F2, $0）
//         std::regex immediateRegex(R"(\d+)");            // 匹配立即数（如 24, 124）
//         std::regex memoryRegex(R"((\d+)\(([^)]+)\))"); // 匹配内存地址（如 200(R0)）

//         // 解析 Operand 1
//         int offset;
//         std::string baseRegister;
//         if (parseMemoryAddress(operand1, offset, baseRegister)) {
//             std::cout << "Operand 1 is a memory address: " << offset << "(" << baseRegister << ")" << std::endl;
//         } else if (std::regex_match(operand1, registerRegex)) {
//             std::cout << "Operand 1 is a register: " << operand1 << std::endl;
//         } else if (std::regex_match(operand1, immediateRegex)) {
//             std::cout << "Operand 1 is an immediate value: " << operand1 << std::endl;
//         }

//         // 解析 Operand 2
//         if (parseMemoryAddress(operand2, offset, baseRegister)) {
//             std::cout << "Operand 2 is a memory address: " << offset << "(" << baseRegister << ")" << std::endl;
//         } else if (std::regex_match(operand2, registerRegex)) {
//             std::cout << "Operand 2 is a register: " << operand2 << std::endl;
//         } else if (std::regex_match(operand2, immediateRegex)) {
//             std::cout << "Operand 2 is an immediate value: " << operand2 << std::endl;
//         }

//         // 解析 Operand 3（如果有）
//         if (!operand3.empty()) {
//             if (parseMemoryAddress(operand3, offset, baseRegister)) {
//                 std::cout << "Operand 3 is a memory address: " << offset << "(" << baseRegister << ")" << std::endl;
//             } else if (std::regex_match(operand3, registerRegex)) {
//                 std::cout << "Operand 3 is a register: " << operand3 << std::endl;
//             } else if (std::regex_match(operand3, immediateRegex)) {
//                 std::cout << "Operand 3 is an immediate value: " << operand3 << std::endl;
//             }
//         }
//     } else {
//         std::cout << "No valid instruction found in line: " << line << std::endl;
//     }
// }