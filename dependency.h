#pragma once
#include <optional>
#include <vector>
#include <string>
using namespace std;

//单个依赖项
struct Dependency {
    int ID_in_Queue;
    string rd;
    string physical_register=0;
    //如果依赖指令很早不一定存在physical register映射,所以还要判断commit没有，commit就读archi寄存器的值。如果没有commit,物理寄存器就没有释放
    //读rd还是 physical_register的值取决于commit没有
    //如果commit了，物理寄存器已经释放，读archi寄存器的值
};

//依赖项列表
struct DependencyList {
    int ID_in_Queue;//指令在instructionQueue中的位置
    //<rs1,rs1 physical register,dependency>
    vector<tuple<string,string,Dependency>> rs_dependency;
};