#pragma once
#include "global.h"
#include "loadstore.h"
#include "forwarding.h"
#include "loadstoretype.h"
#include "common.h"
//因为memory Hazard load after store,所以需要严格执行检查决定是否执行fld
//1.检查ROBfld到head是否存在fsd,不存在就加入就正常写寄存器(都是没commit的指令),memory hazard stall
//2.如果fld ROB前面存在fsd且fsd还没done, 不write back这个指令，memory hazard stall
//3.如果fld ROB前面存在fsd且fsd done,检查是否有地址冲突，没有就正常写寄存器，有地址冲突，那这个指令的第一个操作数直接forwarding给fld,且fld write back完成

class WriteBack{
    private:int nb=NB;
    public:
    WriteBack();
    void writeBack();
    void printCompleteQueue();
};