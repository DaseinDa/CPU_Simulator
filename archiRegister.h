#pragma once
#include "common.h"
#include <deque>
using namespace std;
class ArchitectureRegister{
    public:
        double value;
        deque<string> physicalRegister;
        ArchitectureRegister() :  value(0.0) {}
        ~ArchitectureRegister() { physicalRegister.clear(); }
};