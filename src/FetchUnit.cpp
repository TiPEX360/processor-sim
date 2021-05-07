#include "FetchUnit.h"

void FetchUnit::tick() {

    if(currentPC->value < 512) {
        Instr n;
        n = INSTR[currentPC->value];

        bool branch = false;
        if(n.opcode >= opcode::BLT && n.opcode <= opcode::JNZ) {
            branch = branchBuffer->predictBranchDynamic(currentPC->value, n);
            if(branch) {
                //update PC
                if(n.opcode >= opcode::BLT && n.opcode <= opcode::B) nextPC->value = n.Rd; //go to branch
                else nextPC->value = currentPC->value + n.Rd;
                
                n.branchTaken = 1;

                n.Rd = currentPC->value + 1; //where branch should go had it not been taken OR hadnt been
            }
            else {
                nextPC->value = currentPC->value + 1;
                n.branchTaken = 0;
                if(n.opcode >= opcode::J && n.opcode <= opcode::JNZ) n.Rd = nextPC->value + n.Rd;
            } 
        }
        else nextPC->value = currentPC->value + 1;
        nextFetched.push(n);
    }
    std::cout << "Queued: " << (int)nextFetched.back().opcode << std::endl;
}

void FetchUnit::update() {
    currentFetched = nextFetched;
}


FetchUnit::FetchUnit(BPB *branchBuffer, Register *currentPC, Register *nextPC, Instr *INSTR) {
    FetchUnit::currentPC = currentPC;
    FetchUnit::nextPC = nextPC;
    FetchUnit::branchBuffer = branchBuffer;
    FetchUnit::INSTR = INSTR;
    FetchUnit::currentFetched = std::queue<Instr>();
    FetchUnit::nextFetched = std::queue<Instr>();
    nextFetched.push({opcode::NOP, 0, 0, 0, true, 0, 0});
}
