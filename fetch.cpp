#include "fetch.h"
Fetch::Fetch(){
        cout<<"Here is the fetch stage, fetch.h file"<<endl;
        };


bool Fetch::dispatch(){
    cout<<"fetch dispatching..., nf="<<nf<<endl;
    for(int i=0;i<nf;i++){
        cout<<"i="<<i<<endl;
    if(Global::instructionset.empty()||Global::fetch_pointer >=Global::instructionset.size()){
            cout<<"All instructions have been taken in the pipeline"<<endl;
            return true;
        }else if(Global::fetchInstructionQueue.size()!=0){
            cout<<"At least one instruction which has been fetched in the previous cycle did not dispatch to decode stage at current cycle."<<endl;
            return false;
            //return fetchStall=fetchInstructionQueue.size()
        }else{
            Instruction fetch_intr = Global::instructionset[Global::fetch_pointer];
            Global::fetchInstructionQueue.push_back(fetch_intr);
            //如果指令是bne还需要另外写代码判断，但现在先不考虑bne的情况
            if(fetch_intr.opcode==InstructionType::bne){//branch prediction
                string target_str = fetch_intr.target.value();
                int target_position=Global::btb.getTargetPosition(fetch_intr.instructionNumber);
                if(target_position!=-1){
                    Global::fetch_pointer=target_position;
                }else{
                    Global::fetch_pointer++;// oredict not taken
                }
            }else{
                Global::fetch_pointer++;
            }
        }
    }
}
// cout << "Fetch构造函数中 instructionset 地址: " << &(this->instructionset) << endl;
// cout << "instructionset size " << (this->instructionset).size()<< endl;
// cout << "dispatch中 instructionset 地址: " << &(this->instructionset) << endl;