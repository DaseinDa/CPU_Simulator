//issue ready RS entries to exec units
#pragma once
#include <optional>
#include <variant>
#include "reservationStation.h"
#include "global.h"
#include "instruction.h"
#include "instruction_type.h"
#include "unit_pip.h"
#include "common.h"
using namespace std;
class Execute{
    public:
    void execute();
    void executeINT();
    void executeLOAD();
    void executeSTORE();
    void executeFPADD();
    void executeFPMULT();
    void executeFPDIV();
    void executeFPBU();
    void insertCompletedEntry(const ReservationStationEntry& entry);
};