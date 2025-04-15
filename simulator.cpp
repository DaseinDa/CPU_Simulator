#include "simulator.h"

const char COMMENT_LINE_PREFIX = '%';
Simulator::Simulator(){
    cout<<"Simulator constructor called"<<endl;
}
bool isCommentLine(const string line) {
    return line[0] == COMMENT_LINE_PREFIX;
}
bool isInsturction(const string line){// const variable is read-only
    return isalpha(line[0]);
}
bool isMemory(const string line){// const variable is read-only
    return isdigit(line[0]);
}
void Simulator::readMemory(string line){
    // 解析指令
    istringstream iss(line);//一定要有空格才能识别,输入文件按照特定格式
    string address_text, value_text;
    int address;
    double value;
    iss >> address_text >> value_text;
    // 去除逗号
    address_text.erase(std::remove(address_text.begin(), address_text.end(), ','), address_text.end());
    value_text.erase(std::remove(value_text.begin(), value_text.end(), ','), value_text.end());
    // if(DebugMode){
    //     cout<<"address_text"<<address_text<<endl;
    //     cout<<"value_text"<<value_text<<endl;
    // }
    address=stoi(address_text);
    value=stof(value_text);
    // Read to memory
    Global::memory_value[address]=value;

}
void Simulator::readInstruction(string line){
    if(DebugMode){
    cout<<"Here is readInstruction"<<endl;
    }
    // 解析标签
    size_t colonPos = line.find(':');
    std::string label = "";
    if (colonPos != std::string::npos) {
        label = line.substr(0, colonPos);
        line = line.substr(colonPos + 1);
        line.erase(0, line.find_first_not_of(" \t"));
        Global::labelMap[label] = index; // 记录标签对应的指令索引
    }
    if(DebugMode){
        printLabelMap();
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
    Global::instructions.emplace_back(type, operands, optLabel, target);
}
Simulator::~Simulator(){
    Global::memory_value.clear();
    Global::labelMap.clear();
    if(DebugMode){
        cout<<"Here is the construct function for Simulator class"<<endl;
    }
}
// void Simulator::initSimulatorPhysicalRegs(){
//     // $0 register is default always to save the 0 address in the whole process
//     physicalRegister[ZERO_REGISTER]=0;
//     for(int i=1; i<RESGITER_NUMBER;i++){
//         string physicalRegisterName="p"+to_string(i);
//         //initialization
//         physicalRegister[physicalRegisterName]=0;
//         //The free list of physical register not used
//         RegisterRenamingFreeList.push_back(physicalRegisterName);
//     }
//     //register to physical register renaming mapping table
//     registerRenamingMapping=unordered_map<string, string>();   
// }
void Simulator::ReadAssemblyFile(char * PathFile){
    string filepath = PathFile;
    string line;
    string currentLabel;
    bool inLabelScope = false;

    // InstructionType l=addi;
    ifstream ifs;
    ifs.open(filepath);
    if(ifs.is_open()){
        cout<< "Reading File..."<<endl;
        // 读取汇编代码
        // size_t index = 0;
        while(getline(ifs, line)){
                cout<<line<<endl;
                line=addSpaceAfterComma(line);
                line.erase(0, line.find_first_not_of(" \t"));
                line.erase(line.find_last_not_of(" \t") + 1);
                if (line.empty()) continue;
                if(isCommentLine(line)){
                    continue;
                }
                else if(isMemory(line)){
                    readMemory(line);
                }
                else if(isInsturction(line)){
                    readInstruction(line);
                    index++;
                }

                // parseAssemblyLine(line,currentLabel,inLabelScope);
        }
    }
    if(DebugMode){
        cout<<"The initialized memory address and value is:"<<endl;
        for(const auto& pair: Global::memory_value){
            cout<<"Address:"<<pair.first<<" Value:"<<pair.second<<endl;
    }
    }
    for (size_t i = 0; i < Global::instructions.size(); ++i) {
        std::cout << "Instruction " << i << ": ";
        cout<<"Instruction Address:"<<getInstructionAddress(i)<<" ";
        Global::instructions[i].print();
        Global::instructions[i].instructionNumber=i;
        cout<<"Instruction Address:"<<getInstructionAddress(i)<<endl;
        }
    
    ifs.close();
    //赋值给instructionset
    Global::instructionset.assign(Global::instructions.begin(),Global::instructions.end());
    if(DebugMode){
        if(Global::instructionset.empty()){
            cout<<"-------------------instructionset fail to copy-----------"<<endl;
        }
    }
}
void Simulator::pipelineGlobalCycle(){
    global_cycle++;
    cout<<"The current global cycle count is:"<<global_cycle<<endl;
    if(Global::instructionset.empty()){
        cout<<"-------------------instructionset fail to copy-----------"<<endl;
    }


    //commit
    //write back
    //memory access
    //execute
    //decode+issue
    //fetch一个cycle只能fetch 4个instruction,fetech不分发是否应该统计为stall
    //fetch最后执行因为需要先检查更早执行的指令是不是先能进入下一个状态释放出早期执行的资源
    //decode+issue
    cout<<"---------------------------------------------------------------------------------------------------------------------------"<<endl;
    // w->writeBack();
    // c->commit();
    // //
    // e->execute();
    // //
    // if(!d->decode()) decodeStall++;
    // i->issue();
    // //
    // if(!f->fetch()) fetchStall++;
    c->commit();
    w->writeBack();
    e->execute();
    i->issue();
    if(!d->decode()) decodeStall++;
    if(!f->fetch()) fetchStall++;


    // e->execute();
    // w->writeBack();
    // c->commit();
}
void Simulator::run(){
    this->d= new Decode();
    this->f= new Fetch();
    this->i= new Issue();
    this->e= new Execute();
    this->w= new WriteBack();
    this->c=new Commit();
    // do pipelineGlobalCycle();
    // while(Global::fetchInstructionQueue.size() > 0 || Global::decodeInstructionQueue.size() > 0 ||  Global::ROBuffer.size()>0);
    // do pipelineGlobalCycle();
    // while(Global::fetchInstructionQueue.size() > 0);
    for(int i=0;i<1000;i++){
        pipelineGlobalCycle();
        // sleep(10);
        debugLogger();
    }
}

void Simulator::debugLogger() {
    cout << "\n================ DEBUG LOGGER @ Cycle " << global_cycle << " ================\n";

    // RS_INT Queue
    cout << "[RS_INT_Queue] Size: " << Global::RS_INT_Queue.size() << endl;
    for (const auto& entry : Global::RS_INT_Queue) {
        cout << "  [ID: " << entry.ID_in_Queue
            << ", Opcode: " << entry.opcode
            << ", DestPR: " << entry.destPhysicalRegister
            << ", DestROB: " << entry.destROB << "]\n"
            << "  Qj: " << (entry.Qj.empty() ? "READY" : entry.Qj)
            << " | Vj: " << entry.Vj << "\n"
            << "  Qk: " << (entry.Qk.empty() ? "READY" : entry.Qk)
            << " | Vk: " << entry.Vk << "\n"
            << endl;
    }
    //RS_FPmult Queue
    cout << "[RS_FPmult_Queue] Size: " << Global::RS_FPmult_Queue.size() << endl;
    for (const auto& entry : Global::RS_FPmult_Queue) {
        cout << "  [ID: " << entry.ID_in_Queue
            << ", Opcode: " << entry.opcode
            << ", DestPR: " << entry.destPhysicalRegister
            << ", DestROB: " << entry.destROB << "]\n"
            << "  Qj: " << (entry.Qj.empty() ? "READY" : entry.Qj)
            << " | Vj: " << entry.Vj << "\n"
            << "  Qk: " << (entry.Qk.empty() ? "READY" : entry.Qk)
            << " | Vk: " << entry.Vk << "\n"
            << endl;
    }
    
    // RS_LOAD Queue
    cout << "[RS_LOAD_Queue] Size: " << Global::RS_LOAD_Queue.size() << endl;
    for (const auto& entry : Global::RS_LOAD_Queue) {
        cout << "  [ID: " << entry.ID_in_Queue << ", A: " << entry.A.value() << "]\n";
    }
    //RS_BU Queue
    cout << "[RS_BU_Queue] Size: " << Global::RS_BU_Queue.size() << endl;
        for (const auto& entry : Global::RS_BU_Queue) {
        cout << "  [ID: " << entry.ID_in_Queue
            << ", Opcode: " << entry.opcode
            << ", DestPR: " << entry.destPhysicalRegister
            << ", DestROB: " << entry.destROB << "]\n"
            << "  Qj: " << (entry.Qj.empty() ? "READY" : entry.Qj)
            << " | Vj: " << entry.Vj << "\n"
            << "  Qk: " << (entry.Qk.empty() ? "READY" : entry.Qk)
            << " | Vk: " << entry.Vk << "\n"
            << endl;
    }
    //BU Queue
    cout << "[BU Queue] Size: " << Global::BUQueue.size() << endl;
    //fetch size
    cout << "[Fetch Queue] Size: " << Global::fetchInstructionQueue.size() << endl;
    //decode size
    cout << "[Decode Queue] Size: " << Global::decodeInstructionQueue.size() << endl;
    // ROB
    cout << "[ROB Buffer] Head: " << Global::robHead << ", Tail: " << Global::robTail << endl;
    for (int i = 0; i < Global::ROBuffer.size(); ++i) {
        const auto& rob_entry = Global::ROBuffer[i];
        cout << "  [ROB " << i << "] ID: " << rob_entry.ID_in_Queue
             << ", Opcode: " << static_cast<int>(rob_entry.opcode)
             << ", Status: " << static_cast<int>(rob_entry.status)
             << ", DestArch: " << (rob_entry.dest_archi_register.has_value() ? rob_entry.dest_archi_register.value() : "N/A")
             << ", DestPR: " << (rob_entry.dest_physical_register.has_value() ? rob_entry.dest_physical_register.value() : "N/A")
             << ", done: " << rob_entry.done << endl;
    }

    cout << "=====================================================================\n";

        std::cout << "\n================= [Physical Register Status] =================\n";
    for (const auto& [prName, prEntry] : Global::renaming_worker.physicalRegister) {
    std::cout << std::setw(6) << prName 
          << " | Ready: " << std::boolalpha << prEntry.isReady 
          << " | Value: " << prEntry.value << std::endl;
    }
    std::cout << "==============================================================\n";
}
