#pragma once
#include "common.h"
using namespace std;
class ArchitectureRegister{
    public:
        bool isReady;
        double value;
        deque<string> physicalRegister;
        ArchitectureRegister() : isReady(false), value(0.0) {}
        ~ArchitectureRegister() { physicalRegister.clear(); }
};