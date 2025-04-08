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
//计算单元的数量
#define UNIT_INT_NUMBER 1
#define UNIT_LOAD_NUMBER 1
#define UNIT_STORE_NUMBER 1
#define UNIT_FPADD_NUMBER 1
#define UNIT_FPMULT_NUMBER 1
#define UNIT_FPDIV_NUMBER 1
#define UNIT_FPBU_NUMBER 1
enum InstructionStatus{
    FETCH,
    DECODE,
    ISSUE,
    EXECUTE,
    WB,
    COMMIT
};