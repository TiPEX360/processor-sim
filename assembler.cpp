#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>

enum opcode {ADD, ADDI, MUL, SUB, LDM, LDC, STM, BLT, BNZ, B, HALT};


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
    if(argc < 3) return 1;

    std::string inPath = argv[1];
    std::string outPath = argv[2];
    std::ifstream in(inPath, std::ifstream::in);
    std::ofstream out(outPath, std::ofstream::out | std::ofstream::binary);

    std::string line;
    // std::getline(in, line);
    while(!in.eof()) {
        std::getline(in, line);
        char instr[4] = {0x00, 0x00, 0x00, 0x00};

        std::vector<std::string> tokens = split(line, ' ');

        if(tokens[0].compare("add") == 0) instr[0] = ADD;
        else if(tokens[0].compare("addi") == 0) instr[0] = ADDI;
        else if(tokens[0].compare("mul") == 0) instr[0] = MUL;
        else if(tokens[0].compare("sub") == 0) instr[0] = SUB;
        else if(tokens[0].compare("ldm") == 0) instr[0] = LDM;
        else if(tokens[0].compare("ldm") == 0) instr[0] = LDC;
        else if(tokens[0].compare("stm") == 0) instr[0] = STM;
        else if(tokens[0].compare("blt") == 0) instr[0] = BLT;
        else if(tokens[0].compare("bnz") == 0) instr[0] = BNZ;
        else if(tokens[0].compare("b") == 0) instr[0] = B;
        else if(tokens[0].compare("halt") == 0) instr[0] = HALT;
        else if(tokens[0].compare("") == 0 ) continue;

        for(int i = 1; i < tokens.size(); i++) {
            instr[i] = atoi(tokens[i].c_str());
        }

        out.write((char *)instr, sizeof(char)*4);
    }

    in.close();
    out.close();

    return 0;
}