#include "ExecutionUnit.h"

ExecutionUnit::ExecutionUnit(bool *halt, int *MEM, int *RF, int *pc) {
    ExecutionUnit::MEM = MEM;
    ExecutionUnit::RF = RF;
    ExecutionUnit::pc = pc;
    ExecutionUnit::halt = halt;
}

int ExecutionUnit::execute(Instr instr) {
    int error = 0;
    //Retrieve o1 and o2
    for(int i = 1; i < 3; i++) {
        if(instr.operands[i].second == REGISTER) instr.operands[i] = std::pair<char, addrMode>(RF[instr.operands[i].first], IMMEDIATE);
    }
    int r = instr.operands[0].first;
    int o1 = instr.operands[1].first;
    int o2 = instr.operands[2].first;

    switch(instr.opcode) {
        case ADD:
            RF[r] = o1 + o2;
            (*pc)++;
            break;
        case MUL:
            RF[r] = o1 * o2;
            (*pc)++;
            break;
        case SUB:
            RF[r] = o1 - o2;
            (*pc)++;
            break;
        case DIV:
            RF[r] = o1 / o2;
            (*pc)++;
            break;
        case LSH:
            RF[r] = o1 << o2;
            (*pc)++;
            break;
        case RSH:
            RF[r] = o1 >> o2;
            (*pc)++;
            break;
        case AND:
            RF[r] = o1 & o2;
            (*pc)++;
            break;
        case OR:
            RF[r] = o1 | o2;
            (*pc)++;
            break;
        case XOR:
            RF[r] = o1 ^ o2;
            (*pc)++;
            break;
        case LD:
            RF[r] = MEM[o1 + o2];
            (*pc)++;
            break;
        case LDC:
            RF[r] = o1 + o2;
            (*pc)++;
            break;
        case ST:
            if(instr.operands[0].second == IMMEDIATE) MEM[o1 + o2] = r;
            else MEM[o1 + o2] = RF[r];
            (*pc)++;
            break;
        case BLT:
            if (o1 < o2) *pc = RF[r];
            else (*pc)++;
            break;
        case BNZ:
            if (o1 != 0) *pc = RF[r];
            else (*pc)++;
            break;
        case B:
            *pc = RF[r];
            break;
        case J:
            (*pc) += r;
            break;
        case JLT:
            if(o1 < o2) (*pc) += r;
            else (*pc)++;
            break;
        case JNZ:
            if(o1 != 0) (*pc) += r;
            else (*pc)++;
            break;
        case CMP:
            RF[r] = std::max(-1, std::min(o1 - o2, 1));
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