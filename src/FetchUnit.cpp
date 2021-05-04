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
    if(pc->value < 512) {
        Instr n;
        n = INSTR[pc->value];
        // std::cout << pc->value << "opcode " << n.opcode << std::endl;
        pc->value++;
        n.npc = pc->value;
        next.push(n);
        // ifid->npc = (*pc);
        // ifid->active = true;
    }
    std::cout << "Queued: " << next.back().opcode << std::endl;
}

void FetchUnit::update() {
    // current = std::vector<Instr>(); //change queue to std::vector for this and use clone().
    // for(int i = 0; i < next.size(); i++) current.push_back(next[i]);
    // current = next.
    current = next;
}


FetchUnit::FetchUnit(Register *pc, Instr *INSTR) {
    FetchUnit::pc = pc;
    FetchUnit::INSTR = INSTR;
    FetchUnit::current = std::queue<Instr>();
    FetchUnit::next = std::queue<Instr>();
    next.push({NOP, 0, 0, 0, true, 0});
}
