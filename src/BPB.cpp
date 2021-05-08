#include "BPB.hpp"

BPB::BPB() {
    for(int i = 0; i < 16; i++) {
        currentBuffer[i].state = 0;
        nextBuffer[i].state = 0;
    }
}

//Called during fetch
bool BPB::predictBranchDynamic(uint32_t pc, Instr i) {
    int index = (pc % 15);
        //Predict
    char state = currentBuffer[index].state;
    if(state == 0) {
        return false;
    }
    if(state == 1) {
        return false;
    }
    if(state == 2) {
        return true;
    }
    else {
        return true;
    }
}

//Called during wb
void BPB::updateBranch(uint32_t pc, bool taken) {
    //multiple branches go through before state updated???
    int index = (pc % 15);

    char state = currentBuffer[index].state;
    if(state == 0) {
        if(taken) currentBuffer[index].state = 1;
    }
    if(state == 1) {
        if(taken) currentBuffer[index].state = 3;
    }
    if(state == 2) {
        if(!taken) currentBuffer[index].state = 0;
    }
    else {
        if(!taken) currentBuffer[index].state = 2;
    }
    // for(int i = 0; i < 16; i++) currentBuffer[i].state = nextBuffer[i].state;
}

void BPB::update() {
    // for(int i = 0; i < 16; i++) currentBuffer[i].state = nextBuffer[i].state;
}