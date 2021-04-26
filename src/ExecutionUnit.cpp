#include "ExecutionUnit.h"

ExecutionUnit::ExecutionUnit(bool *halt, PipelineRegister *idex, PipelineRegister *exmem) {
    ExecutionUnit::halt = halt;
    ExecutionUnit::idex = idex;
    ExecutionUnit::exmem = exmem;
}

int ExecutionUnit::execute() {
    
    if(exmem->cond) {
        std::cout << "EXEC FLUSH" << std::endl;
        idex->cond = true;

        exmem->cond = false;

        exmem->active = false;
        idex->active = false;
        return 1;
    }
    
    (*exmem) = (*idex);
    
    
    int error = 0;
    opcode opcode = idex->cir.opcode;
    int Rd = idex->Rd;
    int Rn = idex->Rn;
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
            out = (int)(Rn / Ri);
            std::cout << "Rd " << Rd << " Rn " << Rn << " Ri " << Ri << std::endl;
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
            out = Rn + Ri;
            break;
        case LDC:
            out = Ri; //Rd?
            break;
        case ST:
            out = Rn + Ri;       
            break;
        case STC:
            out = Ri;
            break;
        case BLT:
            exmem->cond = (Rn < Ri);
            out = Rd;
            break;
        case BNZ:
            exmem->cond = (Ri != 0);
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
            exmem->cond = (Rn < Ri);
            out = (idex->npc) + Rd;
            // std::cout << "JLT" << idex->Rd << idex->Rn << idex->Ri << " " << exmem->cond << " " << out << std::endl;
            break;
        case JNZ:
            // if(Ri != 0) (*pc) += Rd;
            // break;
            exmem->cond = (Ri != 0);
            out = (idex->npc) + Rd;
            std::cout << "EXEC// " << Ri << " != 0 : " << exmem->cond << std::endl;
            // std::cout << "JNZ " << idex->Rd << " " << idex->Rn << " " << idex->Ri << std::endl;
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
    std::cout << "EXEC// out: " << exmem->ALUOut << std::endl;
    exmem->active = true;
    idex->active = false;


    return 0;
}