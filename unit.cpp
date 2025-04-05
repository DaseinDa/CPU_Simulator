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
int Unit::getUnitLatency(UnitType unit){
    switch(unit){
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
int Unit::getInstrLatency(InstructionType instr_type){
    UnitType unitType = getUnitType(instr_type);
    return getUnitLatency(unitType);
}

Unit::Unit(UnitType unit){
    unit_function = unit;
    countLatency = getUnitLatency(unit);
}
Unit::~Unit(){}
void Unit::busy(){//只有non-pipelined的单元需要busy, pipelined的单元不需要busy
    isAvailable = false;
}
void Unit::free(){//只有non-pipelined的单元需要free, pipelined的单元不需要free
    isAvailable = true;
    countLatency = getUnitLatency(unit_function);//恢复countLatency
}
void Unit::calculateInCycle(){
        countLatency--;//Issue不占用cycle,所以一设置为busy,马上发送到execute在同一个cycle里countLatency--
        //在每个countLatency--后判断if countLatency==0,就free了
}