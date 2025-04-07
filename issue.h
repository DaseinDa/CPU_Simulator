#pragma once
#include "instruction.h"
#include "reservationStation.h"
#include "common.h"
#include "global.h"
#include "unit.h"



class Issue{
    private:
        int nw=NW;
    public:
        Issue();
        ~Issue();
        bool issue();
};