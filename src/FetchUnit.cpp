#include "FetchUnit.h"

Instr FetchUnit::initInstr(Instr n) {
    bool branch = true;
    if(n.opcode >= opcode::BLT && n.opcode <= opcode::JNZ) {
        branch = branchBuffer->predictBranchDynamic(currentPC->value, n);
        n.bpc = currentPC->value;
        if(branch) {
            //update PC
            if(n.opcode >= opcode::BLT && n.opcode <= opcode::B) nextPC->value = n.Rd; //go to branch
            else nextPC->value = currentPC->value + n.Rd + 1;
            
            n.branchTaken = 1;

            n.Rd = currentPC->value + 1; //where branch should go had it not been taken OR hadnt been
        }
        else {
            // int temptNextPC = currentPC->value + 1;
            nextPC->value = currentPC->value + 1;
            n.branchTaken = 0;
            if(n.opcode >= opcode::J && n.opcode <= opcode::JNZ) n.Rd = currentPC->value + n.Rd + 1;
            else n.Rd = n.Rd;
        } 
    }
    else nextPC->value = currentPC->value + 1;
}

void FetchUnit::tick() {
    if(currentPC->value < 512) {
        Instr n[4];
        for(int i = 0; i < 4; i++) {
            n[i] = INSTR[currentPC->value + i];
            n[i] = initInstr(n[i]);
            nextFetched[i].push(n[i]);
        }

    }
    // std::cout << "Queued: " << (int)nextFetched.back().opcode << std::endl;
}

void FetchUnit::update() {
    currentFetched = nextFetched;
}


FetchUnit::FetchUnit(BPB *branchBuffer, Register *currentPC, Register *nextPC, Instr *INSTR) {
    FetchUnit::currentPC = currentPC;
    FetchUnit::nextPC = nextPC;
    FetchUnit::branchBuffer = branchBuffer;
    FetchUnit::INSTR = INSTR;
    for(int i = 0; i < 4; i++) FetchUnit::currentFetched[i] = std::queue<Instr>();
    for(int i = 0; i < 4; i++) FetchUnit::nextFetched[i] = std::queue<Instr>();
    for(int i = 0; i < 4; i++) nextFetched[i].push({opcode::NOP, 0, 0, 0, true, 0, 0, 0});
}
