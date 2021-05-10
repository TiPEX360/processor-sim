#include "FetchUnit.h"

Instr FetchUnit::initInstr(Instr n) {
    return n;
}

void FetchUnit::tick() {
    if(currentPC->value < 512) {
        Instr n;
        int pc = currentPC->value;
        for(int i = 0; i < SCALE_WIDTH; i++) {
            n = INSTR[pc];

            if(n.opcode >= Opcode::BLT && n.opcode <= Opcode::JNZ) {
                n.bpc = pc;
                if(branchBuffer->predictBranchDynamic(pc, n, PREDICTIONTYPE)) {
                // if(false) {
                    int dest = n.Rd;
                    n.Rd = pc + 1; //where branch should go had it not been taken OR hadnt been

                    //update PC
                    if(n.opcode >= Opcode::BLT && n.opcode <= Opcode::B) {
                        pc = dest;
                    }
                    else pc = pc + dest + 1;
                    
                    n.branchTaken = 1;
                }
                else {
                    pc++;
                    n.branchTaken = 0;
                    if(n.opcode >= Opcode::J && n.opcode <= Opcode::JNZ) n.Rd = pc + n.Rd;
                    else n.Rd = n.Rd;
                } 
            }
            else {
                pc++;
            }
            nextFetched.push_back(n);
        }
        nextPC->value = pc;

    }
}

void FetchUnit::update() {
    currentFetched = nextFetched;
}


FetchUnit::FetchUnit(BPB *branchBuffer, Register *currentPC, Register *nextPC, Instr *INSTR) {
    FetchUnit::currentPC = currentPC;
    FetchUnit::nextPC = nextPC;
    FetchUnit::branchBuffer = branchBuffer;
    FetchUnit::INSTR = INSTR;
    for(int i = 0; i < SCALE_WIDTH; i++) nextFetched.push_back({Opcode::NOP, 0, 0, 0, true, 0, 0, 0});
    for(int i = 0; i < SCALE_WIDTH; i++) currentFetched.push_back({Opcode::NOP, 0, 0, 0, true, 0, 0, 0});
}
