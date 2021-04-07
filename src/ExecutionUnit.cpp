#include "ExecutionUnit.h"

ExecutionUnit::ExecutionUnit(bool *halt, int *MEM, int *RF, int *pc) {
    ExecutionUnit::MEM = MEM;
    ExecutionUnit::RF = RF;
    ExecutionUnit::pc = pc;
    ExecutionUnit::halt = halt;
}

int ExecutionUnit::execute(Instr instr) {
    int error = 0;

    //Retrieve Rn and Ri
    int Rd = instr.Rd;
    int Rn = instr.Rn;
    int Ri;
    if(instr.immediate) Ri = instr.Ri;
    else Ri = RF[instr.Ri];


    switch(instr.opcode) {
        case ADD:
            RF[Rd] = Rn + Ri;
            (*pc)++;
            break;
        case MUL:
            RF[Rn] = Rn * Ri;
            (*pc)++;
            break;
        case SUB:
            RF[Rd] = Rn - Ri;
            (*pc)++;
            break;
        case DIV:
            RF[Rd] = Rn / Ri;
            (*pc)++;
            break;
        case LSH:
            RF[Rd] = Rn << Ri;
            (*pc)++;
            break;
        case RSH:
            RF[Rd] = Rn >> Ri;
            (*pc)++;
            break;
        case AND:
            RF[Rd] = Rn & Ri;
            (*pc)++;
            break;
        case OR:
            RF[Rd] = Rn | Ri;
            (*pc)++;
            break;
        case XOR:
            RF[Rd] = Rn ^ Ri;
            (*pc)++;
            break;
        case LD:
            RF[Rd] = MEM[Rn + Ri];
            (*pc)++;
            break;
        case LDC:
            RF[Rd] = Rn + Ri;
            (*pc)++;
            break;
        case ST:
            MEM[Rn + Ri] = RF[Rd];
            (*pc)++;
            break;
        case STC:
            MEM[Rn + Ri] = Rd;
            (*pc)++;
            break;
        case BLT:
            if (Rn < Ri) *pc = RF[Rd];
            else (*pc)++;
            break;
        case BNZ:
            if (Rn != 0) *pc = RF[Rd];
            else (*pc)++;
            break;
        case B:
            *pc = RF[Rd];
            break;
        case J:
            (*pc) += Rd;
            break;
        case JLT:
            if(RF[Rn] < Ri) (*pc) += Rd;
            else (*pc)++;
            break;
        case JNZ:
            if(RF[Rn] != 0) (*pc) += Rd;
            else (*pc)++;
            break;
        case CMP:
            RF[Rd] = std::max(-1, std::min(RF[Rn] - Ri, 1));
            (*pc)++;
            break;
        case HALT:
            *halt = true;
            break;
        default:
            std::cout << "Error: Opcode not recognised: " << instr.opcode << std::endl;
            error = 1;
            break;
    }

    return 0;
}