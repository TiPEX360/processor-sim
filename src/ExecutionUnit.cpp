#include "ExecutionUnit.h"

ExecutionUnit::ExecutionUnit(bool *halt, PipelineRegister *idex, PipelineRegister *exmem) {
    ExecutionUnit::halt = halt;
    ExecutionUnit::idex = idex;
    ExecutionUnit::exmem = exmem;
}

int ExecutionUnit::execute() {
    (*exmem) = (*idex);
    exmem->active = false;
    exmem->cond = false;
    int error = 0;
    opcode opcode = idex->cir.opcode;
    int Rn = idex->Rn;
    int Rd = idex->Rd;
    int Ri = idex->Ri;
    int out;



    switch(opcode) {
        case NOP:
            break;
        case ADD:
            out = Rn + Ri;
            break;
        case MUL:
            out = Rn * Ri;
            break;
        case SUB:
            out = Rn - Ri;
            break;
        case DIV:
            out = Rn / Ri;
            break;
        case LSH:
            out = Rn << Ri;
            break;
        case RSH:
            out = Rn >> Ri;
            break;
        case AND:
            out = Rn & Ri;
            break;
        case OR:
            out = Rn | Ri;
            break;
        case XOR:
            out = Rn ^ Ri;
            break;
        case LD:
            // RF[Rd] = MEM[RF[Rn] + Ri];
            // (*pc)++;
            out = Rn + Ri;
            break;
        case LDC:
            // RF[Rd] = Ri;
            out = Ri;
            // (*pc)++;
            break;
        case ST:
            // MEM[RF[Rn] + Ri] = RF[Rd];
            out = Rn + Ri;       
            break;
        case STC:
            // MEM[Ri] = Rd;
            out = Ri;
            break;
        case BLT:
            // if (RF[Rn] < Ri) *pc = RF[Rd];
            // else 
            exmem->cond = (Rn < Ri);
            out = Rd;
            break;
        case BNZ:
            // if (Ri != 0) *pc = RF[Rd];
            // else 
            exmem->cond = (Rn != 0);
            out = Rd;
            break;
        case B:
            exmem->cond = true;
            out = Rd;
            break;
        case J:
            exmem->cond = true;
            out = (idex->npc) + Rd;
            break;
        case JLT:
            // if(RF[Rn] < Ri) (*pc) += Rd;
            // else 
            exmem->cond = (Rn < Ri);
            out = (idex->npc) + Rd;
            std::cout << "JLT" << idex->Rd << idex->Rn << idex->Ri << " " << exmem->cond << " " << out << std::endl;
            break;
        case JNZ:
            // if(Ri != 0) (*pc) += Rd;
            // break;
            exmem->cond = (Rn != 0);
            out = (idex->npc) + Rd;
            break;
        case CMP:
            // RF[Rd] = std::max(-1, std::min(RF[Rn] - Ri, 1));
            out = std::max(-1, std::min(Rn - Ri, 1));
            break;
        case HALT:
            *halt = true;
            break;
        default:
            std::cout << "Error: Opcode not recognised: " << opcode << std::endl;
            error = 1;
            break;
    }

    exmem->ALUOut = out;

    idex->active = false;
    exmem->active = true;


    return 0;
}