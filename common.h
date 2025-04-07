#pragma once
#define RESGITER_NUMBER 32
#define ZERO_REGISTER "$0"
#define RS_INT_SIZE 4
#define RS_LOAD_SIZE 2
#define RS_STORE_SIZE 2
#define RS_FPADD_SIZE 3
#define RS_FPMULT_SIZE 2
#define RS_FPDIV_SIZE 1
#define RS_FPBU_SIZE 2

enum InstructionStatus{
    FETCH,
    DECODE,
    ISSUE,
    RS_INT,
    RS_LOAD,
    RS_STORE,
    RS_FPADD,
    RS_FPMULT,
    RS_FPDIV,
    RS_FPBU,
    EXECUTE,
    WB,
    COMMIT
};