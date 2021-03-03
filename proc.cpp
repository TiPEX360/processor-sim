// Partial pseudo code for the interim submission, written in C
// Not shown - Fetch() and Decode() stages
// Only a small selection of simple instructions shown
#include <iostream>
#include <fstream>


enum opcode {MOV, ADD, ADDI, MUL, SUB, LOAD, STORE, BRANCH_LT, BRANCH_NZ, BRANCH, HALT};

struct state {

};

struct instr {
    char opcode;
    signed char r;
    signed char a1;
    signed char a2;
    uint16_t target_addr; //use separate register to combine a1 and a2 range
};

int Execute(int opcode, int r, int a1, int a2, int *RF, int* MEM, int *PC, int target_addr, int *finished) {
	int error = 0;
	switch(opcode) {
		case ADD:
			RF[r] = RF[a1] + RF[a2];
            (*PC)++;
            break;
        case ADDI:
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
		case LOAD:
			RF[r] = MEM[ RF[a1] + RF[a2] ];
            (*PC)++;
            break;
		case STORE:
			MEM[ RF[a1] + RF[a2] ] = RF[r];
            (*PC)++;
            break;
		case BRANCH_LT:
			if (RF[a1] < RF[a2]) *PC = target_addr;
            break;
		case BRANCH_NZ:
			if (a1 != 0) *PC = target_addr;
            break;
		case BRANCH:
			*PC = target_addr;
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
    if((i.opcode >= BRANCH_LT) && (i.opcode <= BRANCH)) i.target_addr = i.a1 + i.a2;
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
		Fetch(CIR, *PC, INSTR); //From memory to CIR in RF

		decoded = Decode(*CIR); //Inspect CIR and determine opcode, r, a1, a2, 

        Execute(decoded.opcode, decoded.r, decoded.a1, decoded.a2, RF, MEM, PC, decoded.target_addr, &finished);
		
        instructions++;
		cycles += 3;
	}

    //Output registers
    for(int r = 0; r < 30; r++) {
        std::cout << "r" << r << ": " << RF[r] << std::endl;
    }
    std::cout << "cir" << ": " << RF[30] << std::endl;
    std::cout << "pc" << ": " << RF[31] << std::endl;
}
