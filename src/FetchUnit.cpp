#include "FetchUnit.h"

FetchUnit::FetchUnit(Instr *cir, int *pc, Instr *INSTR) {
    FetchUnit::cir = cir;
    FetchUnit::pc = pc;
    FetchUnit::INSTR = INSTR;
}

int FetchUnit::fetch() {
    int error = 0;
    if(*pc < 512) *cir = INSTR[*pc];
    else {
        std::cout << "Error: PC out of Memory range: " << *pc << std::endl;
        error = 1;
    }
    return error;
}