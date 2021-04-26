#include "FetchUnit.h"

FetchUnit::FetchUnit(int *pc, Instr *INSTR, PipelineRegister *ifid, PipelineRegister *exmem) {
    FetchUnit::pc = pc;
    FetchUnit::INSTR = INSTR;
    FetchUnit::ifid = ifid;
    FetchUnit::exmem = exmem;
}

int FetchUnit::fetch() {
    int error = 0;
    (*ifid) = PipelineRegister();

    if((*pc) < 512) {
        ifid->cir = INSTR[*pc];
        std::cout << "FETCH// PC: " << *pc << std::endl;
        (*pc)++;
        ifid->npc = (*pc);
        //ORDER OF THESE MIGHT HAVE TO CHANGE WITH PIPELINING

        ifid->active = true;
    } 
    else {
        std::cout << "Error: PC out of Memory range: " << *pc << std::endl;
        error = 1;
    }
    return error;
}