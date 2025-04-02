#include "fetch.h"
Fetch::Fetch(deque<Instruction> & instructionset,deque<Instruction> & fetchInstructionQueue, BTB& btb, const int nf, const unordered_map<string, size_t> labelMap):instructionset(instructionset),fetchInstructionQueue(fetchInstructionQueue),btb(btb),nf(nf), labelMap(labelMap){
        cout<<"Here is the fetch stage, fetch.h file"<<endl;
        };


bool Fetch::dispatch(){
    cout<<"fetch dispatching..., nf="<<nf<<endl;
    for(int i=0;i<nf;i++){
        cout<<"i="<<i<<endl;
    if(instructionset.empty()||fetch_pointer >=instructionset.size()){
            cout<<"All instructions have been taken in the pipeline"<<endl;
            return true;
        }else if(fetchInstructionQueue.size()!=0){
            cout<<"At least one instruction which has been fetched in the previous cycle did not dispatch to decode stage at current cycle."<<endl;
            return false;
            //return fetchStall=fetchInstructionQueue.size()
        }else{
            Instruction fetch_intr = instructionset[fetch_pointer];
            fetchInstructionQueue.push_back(fetch_intr);
            //如果指令是bne还需要另外写代码判断，但现在先不考虑bne的情况
            if(fetch_intr.opcode==InstructionType::bne){//branch prediction
                string target_str = fetch_intr.target.value();
                bool btb_predict=btb.btbMap[getInstructionAddress(fetch_intr.instructionNumber)].predict();
                //如果代码中存在该label，且预测为taken，则跳转
                if(labelMap.count(target_str)>0 && btb_predict){
                fetch_pointer=labelMap.at(target_str);
                }else{
                    fetch_pointer++;
                }
            }else{
                fetch_pointer++;
            }
        }
    }
}
// cout << "Fetch构造函数中 instructionset 地址: " << &(this->instructionset) << endl;
// cout << "instructionset size " << (this->instructionset).size()<< endl;
// cout << "dispatch中 instructionset 地址: " << &(this->instructionset) << endl;