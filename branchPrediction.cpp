#include "branchPrediction.h"

bool BranchPredict::predict(){
    if(stage==PREDICT_WEAK_TAKEN || stage==PREDICT_STRONG_TAKEN) return true;
    else return false;
}

void BranchPredict::update(bool finalTaken){
    //if prediction is correct, do nothing
    if(finalTaken==predict()){
        switch(stage){
            case PREDICT_WEAK_TAKEN:
                stage = PREDICT_STRONG_TAKEN;
                break;
            case PREDICT_STRONG_TAKEN:
                break;
            case PREDICT_WEAK_NOT_TAKEN:
                stage = PREDICT_STRONG_NOT_TAKEN;
                break;
            case PREDICT_STRONG_NOT_TAKEN:
                break;
        }
    }else{
        //if prediction is wrong, update stage
        switch(stage){
            case PREDICT_WEAK_TAKEN:
                stage = PREDICT_WEAK_NOT_TAKEN;
                break;
            case PREDICT_STRONG_TAKEN:
                stage = PREDICT_WEAK_TAKEN;
                break;
            case PREDICT_WEAK_NOT_TAKEN:
                stage = PREDICT_STRONG_NOT_TAKEN;
                break;
            case PREDICT_STRONG_NOT_TAKEN:
                stage = PREDICT_WEAK_NOT_TAKEN;
                break;
        }
    }
}
        