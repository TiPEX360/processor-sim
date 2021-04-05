#include "instr.h"
#include <iostream>

class FetchUnit {
private:
    Instr *cir;
    int *pc;
    Instr *INSTR;
public:
    FetchUnit(Instr *cir, int *pc, Instr *INSTR);
    int fetch();
};