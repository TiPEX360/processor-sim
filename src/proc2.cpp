#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "FetchUnit.h"
#include "DecodeUnit.h"
#include "ExecutionUnit.h"

std::vector<std::string> split(const std::string &line, char delimiter) {
	std::string haystack = line;
	std::vector<std::string> tokens;
	size_t pos;
	while ((pos = haystack.find(delimiter)) != std::string::npos) {
		tokens.push_back(haystack.substr(0, pos));
		haystack.erase(0, pos + 1);
	}
	// Push the remaining chars onto the vector
	tokens.push_back(haystack);
	return tokens;
}

void loadProgram(const char *path, Instr *INSTR) {
    std::ifstream in(path, std::ifstream::in);
    std::string line;
    int instrIndex = 0;
    while(std::getline(in, line)) {
        
        Instr instr;
        for(int i = 0; i < 3; i++) {
            instr.operands[i] = std::pair<char, addrMode>(0, IMMEDIATE);
        }

        std::vector<std::string> tokens = split(line, ' ');
        
        if(tokens[0].compare("add") == 0) instr.opcode = ADD;
        else if(tokens[0].compare("mul") == 0) instr.opcode = MUL;
        else if(tokens[0].compare("sub") == 0) instr.opcode = SUB;
        else if(tokens[0].compare("div") == 0) instr.opcode = DIV;
        else if(tokens[0].compare("lsh") == 0) instr.opcode = LSH;
        else if(tokens[0].compare("rsh") == 0) instr.opcode = RSH;
        else if(tokens[0].compare("and") == 0) instr.opcode = AND;
        else if(tokens[0].compare("or") == 0) instr.opcode = OR;
        else if(tokens[0].compare("xor") == 0) instr.opcode = XOR;
        else if(tokens[0].compare("ld") == 0) instr.opcode = LD;
        else if(tokens[0].compare("ldc") == 0) instr.opcode = LDC;
        else if(tokens[0].compare("st") == 0) instr.opcode = ST;
        else if(tokens[0].compare("blt") == 0) instr.opcode = BLT;
        else if(tokens[0].compare("bnz") == 0) instr.opcode = BNZ;
        else if(tokens[0].compare("b") == 0) instr.opcode = B;
        else if(tokens[0].compare("j") == 0) instr.opcode = J;  
        else if(tokens[0].compare("jlt") == 0) instr.opcode = JLT;
        else if(tokens[0].compare("jnz") == 0) instr.opcode = JNZ;
        else if(tokens[0].compare("cmp") == 0) instr.opcode = CMP;
        else if(tokens[0].compare("halt") == 0) instr.opcode = HALT;
        else continue;

        bool comment = false;
        for(int i = 1; i < tokens.size() && !comment && tokens[i].size() > 0; i++) {
            if(tokens[i][0] == '/') comment = true;
            else {
                std::string numberPart = tokens[i].substr(1, tokens[i].size() - 1);
                if(tokens[i].compare("lr") == 0) instr.operands[i - 1] = std::pair<char, addrMode>(29, REGISTER);
                else if(tokens[i].compare("pc") == 0) instr.operands[i - 1] = std::pair<char, addrMode>(30, REGISTER);
                else if(tokens[i][0] == 'r') instr.operands[i - 1] = std::pair<char, addrMode>(atoi(numberPart.c_str()), REGISTER);
                else if(tokens[i][0] == '#') instr.operands[i - 1] = std::pair<char, addrMode>(atoi(numberPart.c_str()), IMMEDIATE);
            }
        }

        INSTR[instrIndex] = instr;
        instrIndex++;
    }

    in.close();
}

int main(int argc, char *argv[]) {
    bool halt = false;
    int cycles = 0;
    
    //Buffers
    int32_t RF[31];
    int32_t MEM[1024];
    Instr INSTR[512];

    //Special purpose register pointers
    int *lr = &RF[29];
	int *pc = &RF[30];
    Instr *cir = new Instr; //RF[31]
    //Units
    FetchUnit fetchUnit(cir, pc, INSTR);
    DecodeUnit decodeUnit(cir);
    ExecutionUnit executionUnit(&halt, MEM, RF, pc);

    if(argc < 2) {
        std::cout << "Error: Missing arguments. Arg1: assembly source" << std::endl;
        return 1;   
    }
    loadProgram(argv[1], INSTR);

    for(int i = 0; i < 1024; i++) {
        MEM[i] = 0;
    }
    
    while(!halt) {
        fetchUnit.fetch();
        decodeUnit.decode();
        executionUnit.execute(*cir);
        cycles += 3;
    }

    for(int i = 0; i < 1024; i++) {
        if (MEM[i] != 0) std::cout << MEM[i] << std::endl;
    }
    return 0;
}