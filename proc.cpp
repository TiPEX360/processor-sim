// Partial pseudo code for the interim submission, written in C
// Not shown - Fetch() and Decode() stages
// Only a small selection of simple instructions shown
#include <iostream>
#include <fstream>


enum opcode {
            ADD, ADDC, MUL, SUB, DIV,
            LSH, RSH, AND, OR, XOR,
            LDM, LDC, LDMC, STO, STM, STMC, 
            BLT, BNZ, B, 
            J, JLT, JNZ,
            CMP, 
            HALT
            };

struct instr {
    char opcode;
    signed char r;
    signed char a1;
    signed char a2;
    // uint16_t target_addr; //use separate register to combine a1 and a2 range
};

int Execute(int opcode, int r, int a1, int a2, int *RF, int* MEM, int *PC, int target_addr, int *finished) {
	int error = 0;
	switch(opcode) {
		case ADD:
			RF[r] = RF[a1] + RF[a2];
            (*PC)++;
            break;
        case ADDC:
            RF[r] = RF[a1] + a2;
            (*PC)++;
            break;
		case MUL:
			RF[r] = RF[a1] * RF[a2];
            (*PC)++;
            break;
        case SUB:
            RF[r] = RF[a1] - RF[a2];
            (*PC)++;
            break;
        case DIV:
            RF[r] = RF[a1] / RF[a2];
            (*PC)++;
            break;
		case LSH:
            RF[r] = RF[a1] << RF[a2];
            (*PC)++;
            break;
		case RSH:
            RF[r] = RF[a1] >> RF[a2];
            (*PC)++;
            break;
		case AND:
            RF[r] = RF[a1] & RF[a2];
            (*PC)++;
            break;
		case OR:
            RF[r] = RF[a1] | RF[a2];
            (*PC)++;
            break;
		case XOR:
            RF[r] = RF[a1] ^ RF[a2];
            (*PC)++;
            break;
		case LDM:
			RF[r] = MEM[RF[a1] + RF[a2]];
            (*PC)++;
            break;
        case LDC:
            RF[r] = a1;
            (*PC)++;
            break;
        case LDMC:
            RF[r] = MEM[RF[a1] + a2];
            (*PC)++;
            break;
		case STM:
			MEM[RF[a1] + RF[a2]] = RF[r];
            (*PC)++;
            break;
        case STO:
            MEM[a1 + a2] = r;
            (*PC)++;
            break;
        case STMC:
			MEM[RF[a1] + a2] = RF[r];
            (*PC)++;
            break;
		case BLT:
			if (RF[a1] < RF[a2]) *PC = RF[r];
            else (*PC)++;
            break;
		case BNZ:
			if (a1 != 0) *PC = *PC = RF[r];
            else (*PC)++;
            break;
		case B:
			*PC = RF[r];
            break;
		case JLT:
			if (RF[a1] < RF[a2]) (*PC) += r;
            else (*PC)++;
            break;
		case JNZ:
			if (a1 != 0) (*PC) += r;
            else (*PC)++;
            break;
		case J:
			(*PC) += r;
            break;
        case CMP:
            RF[r] = std::max(-1, std::min(RF[a1] - RF[a2], 1));
            break;
		case HALT:
			*finished = true;
            break;
		default:
			std::cout << "Error: Opcode not recognised: " << opcode << std::endl;
            error = 1;
            break;
	}
	return error;
}

int Fetch(int *CIR, int PC, int *INSTR) {
    int error = 0;
    if(PC < 512) *CIR = INSTR[PC];
    else {
        std::cout << "Error: PC out of Memory range: " << PC << std::endl;
        error = 1;
    }
    return error;
}

instr Decode(int CIR) {
    instr i;
    i.opcode = (CIR >> 24);
    i.r = (CIR & 0x00FF0000) >> 16;
    i.a1 = (CIR & 0x0000FF00) >> 8;
    i.a2 = (CIR & 0x000000FF);
    // if((i.opcode >= BLT) && (i.opcode <= B)) i.target_addr = i.a1 + i.a2;
    return i;
}

void loadProgram(const char *path, int *INSTR) {
    std::ifstream in(path, std::ifstream::in | std::ifstream::binary);
    
    int instrIndex = 0;
    char instr[4];
    in.read(instr, sizeof(char) * 4);
    INSTR[instrIndex] = (int)((instr[0] << 24) | (instr[1] << 16) | (instr[2] << 8) | instr[3]);
    instrIndex++;
    while(!in.eof()) {
        in.read(instr, 4);
        INSTR[instrIndex] = (int)((instr[0] << 24) | (instr[1] << 16) | (instr[2] << 8) | instr[3]);
        instrIndex++;
    }
}

int main(int argc, char *argv[]) {
	int finished = 0;
	int cycles = 0;
	int instructions = 0;

	int RF[32];
	int MEM[1024];
	int INSTR[512];

    //Special purpose pointers
    int *lr = &RF[29];
    int *CIR = &RF[30];
	int *PC = &RF[31];

    //RESET
    for(int i = 0; i < 32; i++) RF[i] = 0;
    for(int i = 0; i < 1024; i++) MEM[i] = 0;
    for(int i = 0; i < 512; i++) INSTR[i] = 0;
    instr decoded;

    //Load program
    if(argc > 1) loadProgram(argv[1], INSTR); 
	
    while(!finished) {
        std::cout << *PC << " " << RF[1] << std::endl;
		Fetch(CIR, *PC, INSTR); //From memory to CIR in RF

		decoded = Decode(*CIR); //Inspect CIR and determine opcode, r, a1, a2, 

        Execute(decoded.opcode, decoded.r, decoded.a1, decoded.a2, RF, MEM, PC, *lr, &finished);
		
        instructions++;
		cycles += 3;
	}

    //Output registers
    // for(int r = 0; r < 30; r++) {
    //     std::cout << "r" << r << ": " << RF[r] << std::endl;
    // }
    // std::cout << "cir" << ": " << RF[30] << std::endl;
    // std::cout << "pc" << ": " << RF[31] << std::endl;

    //Output memory
    for(int i = 0; i < 1024; i++) {
        if(MEM[i] != 0) {
            std::cout << "m" << i << ": " << MEM[i] << std::endl;
        }
    }
    std::cout << "Cycles: " << cycles << std::endl;
}
