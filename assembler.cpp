#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>

enum opcode {
            ADD, ADDC, MUL, SUB, DIV,
            LSH, RSH, AND, OR, XOR,
            LDM, LDC, LDMC, STO, STM, STMC, 
            BLT, BNZ, B, 
            J, JLT, JNZ,
            CMP, 
            HALT
            };


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
        else if(tokens[0].compare("addc") == 0) instr[0] = ADDC;
        else if(tokens[0].compare("mul") == 0) instr[0] = MUL;
        else if(tokens[0].compare("sub") == 0) instr[0] = SUB;
        else if(tokens[0].compare("div") == 0) instr[0] = DIV;
        else if(tokens[0].compare("lsh") == 0) instr[0] = LSH;
        else if(tokens[0].compare("rsh") == 0) instr[0] = RSH;
        else if(tokens[0].compare("and") == 0) instr[0] = AND;
        else if(tokens[0].compare("or") == 0) instr[0] = OR;
        else if(tokens[0].compare("xor") == 0) instr[0] = XOR;
        else if(tokens[0].compare("ldm") == 0) instr[0] = LDM;
        else if(tokens[0].compare("ldmc") == 0) instr[0] = LDMC;
        else if(tokens[0].compare("ldc") == 0) instr[0] = LDC;
        else if(tokens[0].compare("stm") == 0) instr[0] = STM;
        else if(tokens[0].compare("sto") == 0) instr[0] = STO;
        else if(tokens[0].compare("stmc") == 0) instr[0] = STMC;
        else if(tokens[0].compare("blt") == 0) instr[0] = BLT;
        else if(tokens[0].compare("bnz") == 0) instr[0] = BNZ;
        else if(tokens[0].compare("b") == 0) instr[0] = B;
        else if(tokens[0].compare("j") == 0) instr[0] = J;
        else if(tokens[0].compare("jlt") == 0) instr[0] = JLT;
        else if(tokens[0].compare("jnz") == 0) instr[0] = JNZ;
        else if(tokens[0].compare("cmp") == 0) instr[0] = CMP;
        else if(tokens[0].compare("halt") == 0) instr[0] = HALT;
        else continue;
                
        bool comment = false;
        for(int i = 1; i < tokens.size() && !comment && tokens[i].size() > 0; i++) {
            if(tokens[i][0] == '/') comment = true;
            else {
                if(tokens[i].compare("lr") == 0) instr[i] = (char)29;
                else if(tokens[i].compare("cir") == 0) instr[i] = (char)30;
                else if(tokens[i].compare("pc") == 0) instr[i] = (char)31;
                else instr[i] = atoi(tokens[i].substr(1, tokens.size() - 2).c_str());
            }
        }

        out.write((char *)instr, sizeof(char)*4);
    }

    in.close();
    out.close();

    return 0;
}