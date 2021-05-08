#include "BPB.hpp"

BPB::BPB() {
    for(int i = 0; i < 16; i++) {
        currentBuffer[i].state = 0;
        nextBuffer[i].state = 0;
    }
}

//Called during fetch
bool BPB::predictBranchDynamic(uint32_t pc, Instr i) {
    int index = (pc & 0x0000001F);
        //Predict
    char state = currentBuffer[index].state;
    if(state == (char)0) {
        return false;
    }
    if(state == (char)1) {
        return false;
    }
    if(state == (char)2) {
        return true;
    }
    else {
        return true;
    }
}

//Called during wb
void BPB::updateBranch() {

}

void BPB::update() {
    for(int i = 0; i < 16; i++) currentBuffer[i] = nextBuffer[i];
}