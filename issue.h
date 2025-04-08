#pragma once
#include "instruction.h"
#include "reservationStation.h"
#include "common.h"
#include "global.h"
#include "unit.h"
#include "ROB.h"
#include "forwarding.h"


class Issue{
    private:
        int nw=NW;
    public:
        Issue();
        ~Issue();
        void issue();
        void insert2RS(ReservationStationEntry& rs_entry);
};