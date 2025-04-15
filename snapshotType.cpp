#include "snapshotType.h"
#include "instruction.h"
Snapshot::~Snapshot(){
    {
        // Clear all containers
        labelMap.clear();
        memory_value.clear();
        instructionQueue.clear();
        fetchInstructionQueue.clear();
        architectureRegisterFile.clear();
        decodeInstructionQueue.clear();
        RS_INT_Queue.clear();
        RS_LOAD_Queue.clear();
        RS_STORE_Queue.clear();
        RS_FPadd_Queue.clear();
        RS_FPmult_Queue.clear();
        RS_FPdiv_Queue.clear();
        RS_BU_Queue.clear();
        ROBuffer.clear();
        INT_pipeline.clear();
        LOAD_pipeline.clear();
        STORE_pipeline.clear();
        FPadd_pipeline.clear();
        FPmult_pipeline.clear();
        FPdiv_pipeline.clear();
        BU_pipeline.clear();
        completeRSQueue.clear();
        BUQueue.clear();
        LoadQueue.clear();
        LoadHazardQueue.clear();
        StoreQueue.clear();
    }
    
}