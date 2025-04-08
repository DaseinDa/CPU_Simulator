#include "renaming.h"
#include "instruction.h"
void RegisterRenaming::initPhysicalRegs(){
    // $0 register is default always to save the 0 address in the whole process
    cout<<"######initPhysicalRegs######"<<endl;
    physicalRegister[ZERO_REGISTER].value=0;
    physicalRegister[ZERO_REGISTER].isReady=true;
    for(int i=0; i<RESGITER_NUMBER;i++){
        string physicalRegisterName="p"+to_string(i);
        //initialization
        physicalRegister[physicalRegisterName].value=0;
        physicalRegister[physicalRegisterName].isReady=true;
        //The free list of physical register not used, only distributed p1-p31
        RegisterRenamingFreeList.push_back(physicalRegisterName);
    }
    //register to physical register renaming mapping table
    registerRenamingMapping=unordered_map<string, string>();   
}
bool RegisterRenaming::isPhysicalRegsAvailable(){
    return !(RegisterRenamingFreeList.empty());
}
string RegisterRenaming::allocatePhysicalReg(string originRegName, bool isDesReg){
    printRegisterRenamingMapping();
    cout<<"Renaming...Distribute physical register..."<<endl;
    if(originRegName==ZERO_REGISTER){
        return ZERO_REGISTER;
    }else if(registerRenamingMapping.count(originRegName)){//是不是已经包含在之前的mapping里了，如果包含且不是destination register就不用分配寄存器，用之前mapping的分配就可以
        if(!isDesReg){
            return registerRenamingMapping[originRegName];//不做任何改变
        }
    }else{

        string freeReg = RegisterRenamingFreeList.front();
        //freeReg是物理寄存器名
        RegisterRenamingFreeList.pop_front();
        registerRenamingMapping[originRegName]=freeReg;
        if(!isValidPhysicalRegister(freeReg)){
            throw runtime_error("Invalid physical register: " + freeReg);
        }
        Global::architectureRegisterFile[originRegName].physicalRegister.push_back(freeReg);
         //如果是rd,需要等待计算Global::architectureRegister的list需要更新或者添加。而且ready需要设置为false
        if(isDesReg){
            physicalRegister[freeReg].isReady=false;
        }else{//如果是源寄存器，而且不存在映射，说明直接读取ready的架构寄存器值，而且本身的物理寄存器也是ready的
            physicalRegister[freeReg].isReady=true;
            physicalRegister[freeReg].value=Global::architectureRegisterFile[originRegName].value;
            //天然如果第一次出现，自动添加到architectureRegisterFile且初始值为0
        }

        return freeReg;
    }
}
//只有当某条指令将它分配为“目标寄存器”时，才将其置为 ready = false
bool RegisterRenaming::instructionRegisterRenaming(Instruction& intr){
    string allocate_phyreg;
     switch(intr.opcode){
        case InstructionType::fld:
            if(!isPhysicalRegsAvailable()){
                    return false;
            }else{
                allocate_phyreg=allocatePhysicalReg(intr.operands[1],false);
                intr.operands[1]=allocate_phyreg;
            }
            if(!isPhysicalRegsAvailable()){
                    return false;
            }else{
                //The detination register of fld is the first register
                allocate_phyreg=allocatePhysicalReg(intr.operands[0],true);
                intr.operands[0]=allocate_phyreg;
                physicalRegister[allocate_phyreg].isReady=false;
            }
            break;
        case InstructionType::fsd:
            //there is no destination register in fsd
            if(!isPhysicalRegsAvailable()){
                    return false;
            }else{
                allocate_phyreg=allocatePhysicalReg(intr.operands[0],false);
                intr.operands[0]=allocate_phyreg;
            }
            if(!isPhysicalRegsAvailable()){
                    return false;
            }else{
                allocate_phyreg=allocatePhysicalReg(intr.operands[1],false);
                intr.operands[1]=allocate_phyreg;
            }
            break;
        case InstructionType::add:
            if(!isPhysicalRegsAvailable()){
                    return false;
            }else{
                allocate_phyreg=allocatePhysicalReg(intr.operands[1],false);
                intr.operands[1]=allocate_phyreg;
            }
            if(!isPhysicalRegsAvailable()){
                    return false;
            }else{
                allocate_phyreg=allocatePhysicalReg(intr.operands[2],false);
                intr.operands[2]=allocate_phyreg;
            }
            if(!isPhysicalRegsAvailable()){
                    return false;
            }else{
                // The detination register of add is the first register
                allocate_phyreg=allocatePhysicalReg(intr.operands[0],true);
                intr.operands[0]=allocate_phyreg;
                physicalRegister[allocate_phyreg].isReady=false;
            }
            break;
        case InstructionType::addi:
            if(!isPhysicalRegsAvailable()){
                    return false;
            }else{
                allocate_phyreg=allocatePhysicalReg(intr.operands[1],false);
                cout<<"####################"<<endl;
                intr.operands[1]=allocate_phyreg;
            }
            if(!isPhysicalRegsAvailable()){
                    cout<<"no physical register available for addi"<<endl;
                    return false;
            }else{
                // The detination register of add is the first register
                allocate_phyreg=allocatePhysicalReg(intr.operands[0],true);
                intr.operands[0]=allocate_phyreg;
                physicalRegister[allocate_phyreg].isReady=false;
            }
            break;
        case InstructionType::slt:
            if(!isPhysicalRegsAvailable()){
                    return false;
            }else{
                allocate_phyreg=allocatePhysicalReg(intr.operands[1],false);
                intr.operands[1]=allocate_phyreg;
            }
            if(!isPhysicalRegsAvailable()){
                    return false;
            }else{
                allocate_phyreg=allocatePhysicalReg(intr.operands[2],false);
                intr.operands[2]=allocate_phyreg;
            }
            if(!isPhysicalRegsAvailable()){
                    return false;
            }else{
                // The detination register of add is the first register
                allocate_phyreg=allocatePhysicalReg(intr.operands[0],true);
                intr.operands[0]=allocate_phyreg;
                physicalRegister[allocate_phyreg].isReady=false;
            }
            break;
        case InstructionType::fadd:
            if(!isPhysicalRegsAvailable()){
                    return false;
            }else{
                allocate_phyreg=allocatePhysicalReg(intr.operands[1],false);
                intr.operands[1]=allocate_phyreg;
            }
            if(!isPhysicalRegsAvailable()){
                    return false;
            }else{
                allocate_phyreg=allocatePhysicalReg(intr.operands[2],false);
                intr.operands[2]=allocate_phyreg;
            }
            if(!isPhysicalRegsAvailable()){
                    return false;
            }else{
                // The detination register of add is the first register
                allocate_phyreg=allocatePhysicalReg(intr.operands[0],true);
                intr.operands[0]=allocate_phyreg;
                physicalRegister[allocate_phyreg].isReady=false;
            }
            break;
        case InstructionType::fsub:
            if(!isPhysicalRegsAvailable()){
                    return false;
            }else{
                allocate_phyreg=allocatePhysicalReg(intr.operands[1],false);
                intr.operands[1]=allocate_phyreg;
            }
            if(!isPhysicalRegsAvailable()){
                    return false;
            }else{
                allocate_phyreg=allocatePhysicalReg(intr.operands[2],false);
                intr.operands[2]=allocate_phyreg;
            }
            if(!isPhysicalRegsAvailable()){
                    return false;
            }else{
                // The detination register of add is the first register
                allocate_phyreg=allocatePhysicalReg(intr.operands[0],true);
                intr.operands[0]=allocate_phyreg;
                physicalRegister[allocate_phyreg].isReady=false;
            }
            break;
        case InstructionType::fmul:
            if(!isPhysicalRegsAvailable()){
                    return false;
            }else{
                allocate_phyreg=allocatePhysicalReg(intr.operands[1],false);
                intr.operands[1]=allocate_phyreg;
            }
            if(!isPhysicalRegsAvailable()){
                    return false;
            }else{
                allocate_phyreg=allocatePhysicalReg(intr.operands[2],false);
                intr.operands[2]=allocate_phyreg;
            }
            if(!isPhysicalRegsAvailable()){
                    return false;
            }else{
                // The detination register of add is the first register
                allocate_phyreg=allocatePhysicalReg(intr.operands[0],true);
                intr.operands[0]=allocate_phyreg;
                physicalRegister[allocate_phyreg].isReady=false;
            }
            break;
        case InstructionType::fdiv:
            if(!isPhysicalRegsAvailable()){
                    return false;
            }else{
                allocate_phyreg=allocatePhysicalReg(intr.operands[1],false);
                intr.operands[1]=allocate_phyreg;
            }
            if(!isPhysicalRegsAvailable()){
                    return false;
            }else{
                allocate_phyreg=allocatePhysicalReg(intr.operands[2],false);
                intr.operands[2]=allocate_phyreg;
            }
            if(!isPhysicalRegsAvailable()){
                    return false;
            }else{
                // The detination register of add is the first register
                allocate_phyreg=allocatePhysicalReg(intr.operands[0],true);
                intr.operands[0]=allocate_phyreg;
                physicalRegister[allocate_phyreg].isReady=false;
            }
            break;
        case InstructionType::bne:
            //There is no destination register in bne
            if(!isPhysicalRegsAvailable()){
                    return false;
            }else{
                allocate_phyreg=allocatePhysicalReg(intr.operands[0],false);
                intr.operands[0]=allocate_phyreg;
            }
            if(!isPhysicalRegsAvailable()){
                    return false;
            }else{
                allocate_phyreg=allocatePhysicalReg(intr.operands[1],false);
                intr.operands[1]=allocate_phyreg;
            }
            break;
        default:
            if(DebugMode){
                cout<<"Instructiontyoe not recoginized at register renaming"<<endl;
            }
            return false;//instructiontype not recognized
     }
}
string RegisterRenaming::getPhysicalRegID(string originRegName){
    if(registerRenamingMapping.count(originRegName)){
        return registerRenamingMapping[originRegName];
    }else{
        throw runtime_error("Physical register not found: " + originRegName);
    }
}
bool RegisterRenaming::isValidPhysicalRegister(string physicalRegisterName){
    // 检查格式是否为 "p" + 数字
    if(physicalRegisterName==ZERO_REGISTER){
        return true;
    }else if (physicalRegisterName.empty() || physicalRegisterName[0] != 'p') {
        return false;
    }

    // 提取数字部分
    string num_str = physicalRegisterName.substr(1);
    if (num_str.empty()) {
        return false;
    }

    // 检查是否全是数字
    for (char c : num_str) {
        if (!std::isdigit(c)) {
            return false;
        }
    }

    // 转换为整数并检查范围
    int index = std::stoi(num_str);
    return index >= 0 && index < RESGITER_NUMBER; // P0 到 P31
}


bool RegisterRenaming::QjQkVjVk(Instruction& instr) {
    //因为QjQkVjVk在renaming之前执行，所以传过来的是全部是架构寄存器
    string Qj;
    string Qk;
    double Vj=0;
    double Vk=0;
    // fld和addi一样只有一个源操作数，的第二个操作数是源操作数。fsd要单独写，因为它第一个第二个都是源操作数
    string Qj_archchi_reg;
    if(instr.opcode==InstructionType::bne || instr.opcode==InstructionType::fsd){
        Qj_archchi_reg=instr.operands[0];
    }else{
        Qj_archchi_reg=instr.operands[1];   
    }

    if (Global::renaming_worker.registerRenamingMapping.count(Qj_archchi_reg)) {
        string Qj_physical_reg=Global::renaming_worker.registerRenamingMapping[Qj_archchi_reg];
        if(Global::renaming_worker.physicalRegister[Qj_physical_reg].isReady){
            Qj="";
            Vj=Global::renaming_worker.physicalRegister[Qj_physical_reg].value;
        }else{
            Qj = Qj_physical_reg;  // 有依赖
        }
    } else {
        if(instr.opcode==InstructionType::bne){
            Vj = Global::architectureRegisterFile[Qj_archchi_reg].value;
            Qj = "";  // 就绪
        }else{
            Vj = Global::architectureRegisterFile[Qj_archchi_reg].value;
            Qj = "";  // 就绪
        }

    }

    // Step 3: 设置 Vk/Qk（双操作数指令才需要）
    if (instr.opcode == InstructionType::add ||instr.opcode == InstructionType::slt || instr.opcode == InstructionType::fadd || instr.opcode == InstructionType::fsub || instr.opcode == InstructionType::fmul || instr.opcode == InstructionType::fdiv) {
        if (Global::renaming_worker.registerRenamingMapping.count(instr.operands[2])) {
            string Qk_physical_reg=Global::renaming_worker.registerRenamingMapping[instr.operands[2]];
            if(Global::renaming_worker.physicalRegister[Qk_physical_reg].isReady){
                Qk="";
                Vk=Global::renaming_worker.physicalRegister[Qk_physical_reg].value;
            }else{
                Qk = Global::renaming_worker.registerRenamingMapping[instr.operands[2]];  // 有依赖
            }
        } else{
            Vk = Global::architectureRegisterFile[instr.operands[2]].value;
            Qk = "";
        } 
    }


    if (instr.opcode == InstructionType::bne || instr.opcode == InstructionType::fsd) {
        if (Global::renaming_worker.registerRenamingMapping.count(instr.operands[1])) {
            string Qk_physical_reg=Global::renaming_worker.registerRenamingMapping[instr.operands[1]];
            if(Global::renaming_worker.physicalRegister[Qk_physical_reg].isReady){
                Qk="";
                Vk=Global::renaming_worker.physicalRegister[Qk_physical_reg].value;
            }else{
                Qk = Qk_physical_reg;  // 有依赖
            }
        } else{
            Vk = Global::architectureRegisterFile[instr.operands[1]].value;
            Qk = "";
        } 
    }

    // if(instr.opcode==InstructionType::addi){
    //     Qk="";
    //     Vk=instr.immediate.value();
    // }
    instr.Qj=Qj;
    instr.Qk=Qk;
    instr.Vj=Vj;
    instr.Vk=Vk;
    return true;
}

void RegisterRenaming::printRegisterRenamingMapping() {
    std::cout << "Register Renaming Mapping:" << std::endl;
    std::cout << "------------------------" << std::endl;
    for (const auto& pair : registerRenamingMapping) {
        std::cout << "Architectural Register: " << pair.first 
                  << " -> Physical Register: " << pair.second 
                  <<"  Status: "<< (physicalRegister[pair.second].isReady ? "READY" : "NOT READY");
                // if (physicalRegister[pair.second].isReady) {
                    std::cout << " Value: " << physicalRegister[pair.second].value;
                // }
                std::cout << endl;
    }
    
    std::cout << "------------------------" << std::endl;
    std::cout << "Total mappings: " << registerRenamingMapping.size() << std::endl;
}
