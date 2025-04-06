#pragma once
#define RESGITER_NUMBER 32
#define ZERO_REGISTER "$0"

enum InstructionStatus{
    FETCH,
    DECODE,
    EXECUTE,
    WB,
    COMMIT
};