#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include "instr.h"

// enum opcode {
//     ADD,  MUL, SUB, DIV,
//     LSH, RSH, AND, OR, XOR,
//     LD, ST, 
//     BLT, BNZ, B, 
//     J, JLT, JNZ,
//     CMP, 
//     HALT
// };

// enum addrMode {
//     REGISTER,
//     IMMEDIATE,
// };


// struct instr {
//     opcode opcode;
//     std::array<std::pair<signed char, addrMode>, 3> operands;
// };

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

int main(int argc, char *argv[]) {
    if(argc < 3) {
        std::cout << "Error: Missing arguments. Arg1: input, Arg2: output" << std::endl;
        return 1;   
    }

    std::string inPath = argv[1];
    std::string outPath = argv[2];
    std::ifstream in(inPath, std::ifstream::in);
    std::ofstream out(outPath, std::ofstream::out | std::ofstream::binary);

    std::string line;
    while(!in.eof()) {
        std::getline(in, line);
        Instr *instr = new Instr;
        for(int i = 0; i < 3; i++) {
            instr->operands[i] = std::pair<signed char, addrMode>(0x0, IMMEDIATE);
        }

        std::vector<std::string> tokens = split(line, ' ');
        
        if(tokens[0].compare("add") == 0) instr->opcode = ADD;
        else if(tokens[0].compare("mul") == 0) instr->opcode = MUL;
        else if(tokens[0].compare("sub") == 0) instr->opcode = SUB;
        else if(tokens[0].compare("div") == 0) instr->opcode = DIV;
        else if(tokens[0].compare("lsh") == 0) instr->opcode = LSH;
        else if(tokens[0].compare("rsh") == 0) instr->opcode = RSH;
        else if(tokens[0].compare("and") == 0) instr->opcode = AND;
        else if(tokens[0].compare("or") == 0) instr->opcode = OR;
        else if(tokens[0].compare("xor") == 0) instr->opcode = XOR;
        else if(tokens[0].compare("ld") == 0) instr->opcode = LD;
        else if(tokens[0].compare("st") == 0) instr->opcode = ST;
        else if(tokens[0].compare("blt") == 0) instr->opcode = BLT;
        else if(tokens[0].compare("bnz") == 0) instr->opcode = BNZ;
        else if(tokens[0].compare("b") == 0) instr->opcode = B;
        else if(tokens[0].compare("j") == 0) instr->opcode = J;  
        else if(tokens[0].compare("jlt") == 0) instr->opcode = JLT;
        else if(tokens[0].compare("jnz") == 0) instr->opcode = JNZ;
        else if(tokens[0].compare("cmp") == 0) instr->opcode = CMP;
        else if(tokens[0].compare("halt") == 0) instr->opcode = HALT;
        else continue;

        bool comment = false;
        for(int i = 1; i < tokens.size() && !comment && tokens[i].size() > 0; i++) {
            if(tokens[i][0] == '/') comment = true;
            else {
                const char *numberPart = tokens[i].substr(1, tokens.size() - 2).c_str();
                if(tokens[i].compare("lr") == 0) instr->operands[i = 1] = std::pair<signed char, addrMode>(29, REGISTER);
                else if(tokens[i].compare("pc") == 0) instr->operands[i = 1] = std::pair<signed char, addrMode>(31, REGISTER);
                else if(tokens[i].compare("r") == 0) instr->operands[i - 1] = std::pair<signed char, addrMode>(atoi(numberPart), REGISTER);
                else if(tokens[i].compare("#") == 0) instr->operands[i - 1] = std::pair<signed char, addrMode>(atoi(numberPart), IMMEDIATE);
            }
        }
        instr->opcode = BNZ;
        out << instr;
    }

    in.close();
    out.close();

    return 0;
}