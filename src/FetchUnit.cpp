#include "FetchUnit.h"

void FetchUnit::tick() {

    if(currentPC->value < 512) {
        Instr n;
        n = INSTR[currentPC->value];

        bool branch = false;
        if(n.opcode >= opcode::BLT && n.opcode <= opcode::JNZ) {
            branch = branchBuffer->predictBranchDynamic(currentPC->value, n);
        }
        if(!branch) nextPC->value = currentPC->value + 1;
        else {
            //branch??!??
            nextPC->value = currentPC->value + 1; //dont take branch anyway ;)
        }
        n.npc = currentPC->value + 1; //save what PC would be had branch not been taken
        next.push(n);
    }
    std::cout << "Queued: " << (int)next.back().opcode << std::endl;
}

void FetchUnit::update() {
    current = next;
}


FetchUnit::FetchUnit(BPB *branchBuffer, Register *currentPC, Register *nextPC, Instr *INSTR) {
    FetchUnit::currentPC = currentPC;
    FetchUnit::nextPC = nextPC;
    FetchUnit::branchBuffer = branchBuffer;
    FetchUnit::INSTR = INSTR;
    FetchUnit::current = std::queue<Instr>();
    FetchUnit::next = std::queue<Instr>();
    next.push({opcode::NOP, 0, 0, 0, true, 0});
}
