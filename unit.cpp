#include "unit.h"

Unit::Unit(){

}
UnitType Unit::getUnitType(InstructionType instr_type){
    switch(instr_type){
        case InstructionType::add:
            return UnitType::INT;
        case InstructionType::addi:
            return UnitType::INT;
        case InstructionType::slt:
            return UnitType::INT;
        case InstructionType::fld:
            return UnitType::LoadStore;
        case InstructionType::fsd:
            return UnitType::LoadStore;
        case InstructionType::fadd:
            return UnitType::FPadd;
        case InstructionType::fsub:
            return UnitType::FPadd;
        case InstructionType::fmul:
            return UnitType::FPmul;
        case InstructionType::fdiv:
            return UnitType::FPdiv;
        case InstructionType::bne:
            return UnitType::BU;
        default:
            throw std::runtime_error("Invalid instruction for recognzing unit type"); 
    }
}
int Unit::getLatency(InstructionType instr_type){
    UnitType unitType = getUnitType(instr_type);
    switch(unitType){
        case UnitType::INT:
            return IntLatency;
        case UnitType::LoadStore:
            return LoadStoreLatency;
        case UnitType::FPadd:
            return FPaddLatency;
        case UnitType::FPmul:
            return FPmulLatency;
        case UnitType::FPdiv:
            return FPdivLatency;
        case UnitType::BU:
            return BULatency;
        default:
            throw std::runtime_error("Invalid unit type for getting latency");
    }
}