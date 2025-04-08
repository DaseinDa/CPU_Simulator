#pragma once
#include "reservationStation.h"

struct PipelineStage {
    ReservationStationEntry rs_entry;
    int remaining_latency;
};
