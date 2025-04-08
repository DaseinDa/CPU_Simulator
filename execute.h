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
#include "historySnapshot.h"
using namespace std;
class Execute{
    public:
    bool execute();
    void executeINT();
    void executeLOAD();
    void executeSTORE();
    void executeFPADD();
    void executeFPMULT();
    void executeFPDIV();
    bool executeBU(int earliest_ID_in_Queue);
    void insertCompletedEntry(const ReservationStationEntry& entry);
    int getEarliestIDIn_RS_BU_Queue();
};