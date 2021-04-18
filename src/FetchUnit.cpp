#include "FetchUnit.h"

FetchUnit::FetchUnit(int *pc, Instr *INSTR, PipelineRegister *ifid, PipelineRegister *exmem) {
    FetchUnit::pc = pc;
    FetchUnit::INSTR = INSTR;
    FetchUnit::ifid = ifid;
    FetchUnit::exmem = exmem;
}

int FetchUnit::fetch() {
    int error = 0;
    if((*pc) < 512) {

        ifid->cir = INSTR[*pc];
        if(exmem->cir.opcode >= BLT && exmem->cir.opcode <= JNZ && exmem->cond) {
            ifid->npc = exmem->ALUOut;
            std::cout << "branch" << *pc << std::endl;
            (*pc) = exmem->ALUOut;
        } else {
            std::cout << "next" << *pc << std::endl;
            ifid->npc = (*pc)++;
            (*pc)++;
        }

        ifid->active = true;
    } 
    else {
        std::cout << "Error: PC out of Memory range: " << *pc << std::endl;
        error = 1;
    }
    return error;
}