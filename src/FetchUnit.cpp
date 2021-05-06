#include "FetchUnit.h"

// int FetchUnit::fetch() {
//     int error = 0;
//     (*ifid) = PipelineRegister();

//     if((*pc) < 512) {
//         ifid->cir = INSTR[*pc];
//         std::cout << "FETCH// PC: " << *pc << std::endl;
//         (*pc)++;
//         ifid->npc = (*pc);
//         //ORDER OF THESE MIGHT HAVE TO CHANGE WITH PIPELINING

//         ifid->active = true;
//     } 
//     else {
//         std::cout << "Error: PC out of Memory range: " << *pc << std::endl;
//         error = 1;
//     }
//     return error;
// }

// void FetchUnit::before() {
//     next = current;
// }

void FetchUnit::tick() {
    // *ifid = PipelineRegister();
    if(currentPC->value < 512) {
        Instr n;
        n = INSTR[currentPC->value];
        // std::cout << pc->value << "opcode " << n.opcode << std::endl;
        bool branch = false;
        if(n.opcode >= BLT && n.opcode <= JNZ) {
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
    std::cout << "Queued: " << next.back().opcode << std::endl;
}

void FetchUnit::update() {
    // current = std::vector<Instr>(); //change queue to std::vector for this and use clone().
    // for(int i = 0; i < next.size(); i++) current.push_back(next[i]);
    // current = next.
    current = next;
}


FetchUnit::FetchUnit(BPB *branchBuffer, Register *currentPC, Register *nextPC, Instr *INSTR) {
    FetchUnit::currentPC = currentPC;
    FetchUnit::nextPC = nextPC;
    FetchUnit::branchBuffer = branchBuffer;
    FetchUnit::INSTR = INSTR;
    FetchUnit::current = std::queue<Instr>();
    FetchUnit::next = std::queue<Instr>();
    next.push({NOP, 0, 0, 0, true, 0});
}
