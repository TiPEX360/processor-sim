#include "FetchUnit.h"

Instr FetchUnit::initInstr(Instr n) {
    // bool branch = true;
    // if(n.opcode >= opcode::BLT && n.opcode <= opcode::JNZ) {
    //     branch = branchBuffer->predictBranchDynamic(currentPC->value, n);
    //     n.bpc = currentPC->value;
    //     if(branch) {
    //         //update PC
    //         if(n.opcode >= opcode::BLT && n.opcode <= opcode::B) nextPC->value = n.Rd; //go to branch
    //         else nextPC->value = currentPC->value + n.Rd + 1;
            
    //         n.branchTaken = 1;

    //         n.Rd = currentPC->value + 1; //where branch should go had it not been taken OR hadnt been
    //     }
    //     else {
    //         // int temptNextPC = currentPC->value + 1;
    //         nextPC->value = currentPC->value + 1;
    //         n.branchTaken = 0;
    //         if(n.opcode >= opcode::J && n.opcode <= opcode::JNZ) n.Rd = currentPC->value + n.Rd + 1;
    //         else n.Rd = n.Rd;
    //     } 
    // }
    // else nextPC->value = currentPC->value + 4;

    return n;
}

void FetchUnit::tick() {
    if(currentPC->value < 512) {
        Instr n;
        int pc = currentPC->value;
        for(int i = 0; i < 4; i++) {
            n = INSTR[pc];

            if(n.opcode >= opcode::BLT && n.opcode <= opcode::JNZ) {
                n.bpc = pc;
                if(branchBuffer->predictBranchDynamic(pc, n)) {
                // if(false) {
                    int dest = n.Rd;
                    n.Rd = pc + 1; //where branch should go had it not been taken OR hadnt been

                    //update PC
                    if(n.opcode >= opcode::BLT && n.opcode <= opcode::B) {
                        pc = dest;
                    }
                    else pc = pc + dest + 1;
                    
                    n.branchTaken = 1;
                }
                else {
                    // int temptNextPC = currentPC->value + 1;
                    // nextPC->value = currentPC->value + 4; /////////////////////////////
                    pc++;
                    n.branchTaken = 0;
                    if(n.opcode >= opcode::J && n.opcode <= opcode::JNZ) n.Rd = pc + n.Rd;
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
    for(int i = 0; i < 4; i++) nextFetched.push_back({opcode::NOP, 0, 0, 0, true, 0, 0, 0});
    for(int i = 0; i < 4; i++) currentFetched.push_back({opcode::NOP, 0, 0, 0, true, 0, 0, 0});
}
