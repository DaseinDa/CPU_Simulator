#pragma once
#include "BranchPredictStage.h"
#include <iostream>
using namespace std;
class BranchPredict{
    private: BranchPredictionStage stage=PREDICT_WEAK_TAKEN;
    public:
    BranchPredict(){
        cout<<"BranchPredictor initialized"<<endl;
    }
    bool predict();
    //update stage based on finally taken or not taken
    void update(bool finalTaken);
};