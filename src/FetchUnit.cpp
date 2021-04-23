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
        if((exmem->cir.opcode >= BLT) && (exmem->cir.opcode <= JNZ) && exmem->cond) {
            // ifid->cir = INSTR[exmem->ALUOut];
            // ifid->npc = exmem->ALUOut; //FETCH CORRECT INSTR HERE
            (*pc) = exmem->ALUOut;
        }
        // } else {
            ifid->cir = INSTR[*pc];
            std::cout << "PC: " << *pc << " " << std::endl;
            ifid->npc = (*pc) + 1;
            (*pc)++;
        // }
        //ORDER OF THESE MIGHT HAVE TO CHANGE WITH PIPELINING

        ifid->active = true;
    } 
    else {
        std::cout << "Error: PC out of Memory range: " << *pc << std::endl;
        error = 1;
    }
    return error;
}