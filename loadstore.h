#pragma once

struct LoadResult {
    int rob_id;
    string prf_id;
    double value;
    int ID_in_Queue;
};


struct PendingLoad {
    int rob_id;
    string prf_id;
    int address;
    int ID_in_Queue;
    bool hazard_checked = false;
};
struct ReadyStore {
    int rob_id;
    string prf_id;
    int address;
    double value;
    int ID_in_Queue;
    bool committed = false;
};
