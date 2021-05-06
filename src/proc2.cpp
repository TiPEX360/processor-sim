#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "FetchUnit.h"
#include "DecodeUnit.h"
#include "ExecutionUnit.h"
#include "ALU.hpp"
#include "LSU.hpp"
#include "BranchUnit.hpp"
#include "MemoryUnit.h"
#include "WriteBackUnit.h"
#include "ReservationStation.h"
#include "ReorderBuffer.hpp"
#include "BPB.hpp"



std::vector<std::string> split(const std::string &line, char delimiter) {
	std::string haystack = line;
	std::vector<std::string> tokens;
	size_t pos;
	while ((pos = haystack.find(delimiter)) != std::string::npos) {
        if(haystack.substr(0, pos).length() > 0 && haystack.substr(0, pos)[0] != '\0') {
		    tokens.push_back(haystack.substr(0, pos));
        }
		haystack.erase(0, pos + 1);
	}
	// Push the remaining chars onto the vector
    if(haystack.length() > 0 && haystack[0] != '\0') {
	    tokens.push_back(haystack);
    }
	return tokens;
}

void loadProgram(const char *path, Instr *INSTR) {
    std::ifstream in(path, std::ifstream::in);
    std::string line;
    int instrIndex = 0;
    while(std::getline(in, line)) {
        
        Instr instr;
        instr.Rd = 0;
        instr.Rn = 0;
        instr.Ri = 0;
        instr.immediate = false;
        instr.npc = 0;
        instr.RSID = 0;

        //Remove comments !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        bool comment = false;
        int i = 0;
        while(i < line.length() && !comment) {
            if(line[i] == '/'){
                line.erase(i);
                comment = true;
            }
            i++;
        }   
        std::vector<std::string> tokens = split(line, ' ');

        // if(tokens.size() > 0) {
            // if(tokens[0].compare("jnz") == 0) std::cout << tokens.size() << tokens[0] << tokens[1] << tokens[2] << std::endl;
        // }
        //Remove empty tokens

        //Replace tokens
        for(int i = 0; i < tokens.size(); i++) {
            if(tokens[i].compare("lr") == 0) tokens[i] = "r29";
            else if(tokens[i].compare("pc") == 0) tokens[i] = "r30";
        }


        if(tokens.size() > 0) {
            //Assign opcode
            if(tokens[0].compare("nop") == 0) instr.opcode = opcode::NOP;
            else if(tokens[0].compare("add") == 0) instr.opcode = opcode::ADD;
            else if(tokens[0].compare("mul") == 0) instr.opcode = opcode::MUL;
            else if(tokens[0].compare("sub") == 0) instr.opcode = opcode::SUB;
            else if(tokens[0].compare("div") == 0) instr.opcode = opcode::DIV;
            else if(tokens[0].compare("lsh") == 0) instr.opcode = opcode::LSH;
            else if(tokens[0].compare("rsh") == 0) instr.opcode = opcode::RSH;
            else if(tokens[0].compare("and") == 0) instr.opcode = opcode::AND;
            else if(tokens[0].compare("or") == 0) instr.opcode = opcode::OR;
            else if(tokens[0].compare("xor") == 0) instr.opcode = opcode::XOR;
            else if(tokens[0].compare("ld") == 0) instr.opcode = opcode::LD;
            else if(tokens[0].compare("ldc") == 0) instr.opcode = opcode::LDC;
            else if(tokens[0].compare("st") == 0) instr.opcode = opcode::ST;
            else if(tokens[0].compare("stc") == 0) instr.opcode = opcode::STC;
            else if(tokens[0].compare("blt") == 0) instr.opcode = opcode::BLT;
            else if(tokens[0].compare("bnz") == 0) instr.opcode = opcode::BNZ;
            else if(tokens[0].compare("b") == 0) instr.opcode = opcode::B;
            else if(tokens[0].compare("j") == 0) instr.opcode = opcode::J;  
            else if(tokens[0].compare("jlt") == 0) instr.opcode = opcode::JLT;
            else if(tokens[0].compare("jnz") == 0) instr.opcode = opcode::JNZ;
            else if(tokens[0].compare("cmp") == 0) instr.opcode = opcode::CMP;
            else if(tokens[0].compare("halt") == 0) instr.opcode = opcode::HALT;
            else continue;

            //Assign operands
            if(tokens.size() > 1) {
                instr.Rd = atoi(tokens[1].substr(1, tokens[1].size() - 1).c_str());
                if(tokens.size() == 3) {
                    if(tokens[2][0] == '#') instr.immediate = true;
                    instr.Ri = atoi(tokens[2].substr(1, tokens[2].size() - 1).c_str());
                }
                else if(tokens.size() == 4) {
                    instr.Rn = atoi(tokens[2].substr(1, tokens[2].size() - 1).c_str());
                    if(tokens[3][0] == '#') instr.immediate = true;
                    instr.Ri = atoi(tokens[3].substr(1, tokens[3].size() - 1).c_str());
                }
            }

            // if(instr.opcode == JNZ) {
            //     std::cout << (int)instr.Rd << " " << (int)instr.Rn << " " << (int)instr.Ri << std::endl;
            // }
            
            // std::cout << tokens[0] << " " << (int)instr.immediate << " " << (int)instr.Rd << " " << (int)instr.Rn << " " << (int)instr.Ri << std::endl;
            

            INSTR[instrIndex] = instr;
            instrIndex++;

        }
    }
    in.close();
}

int main(int argc, char *argv[]) {
    bool halt = false;
    int cycles = 0;
    
    //Buffers
    Register currentRF[31];
    Register nextRF[31];
    int32_t currentMEM[1024];
    int32_t nextMEM[1024];
    Instr INSTR[512];
    for(int i = 0; i < 31; i++) {
        currentRF[i] = (Register){0, -1};
        nextRF[i] = (Register){0, -1};
    }
    for(int i = 0; i < 1024; i++) {
        currentMEM[i] = 0;
        nextMEM[i] = 0;
    }
    for(int i = 0; i < 512; i++) {
        INSTR[i] = (Instr){opcode::NOP, 0, 0, 0, true, 0, 0};
    }

    //Special purpose register pointers
    Register *currentLR = &currentRF[29];
	Register *currentPC = &currentRF[30];
    Register *nextLR = &nextRF[29];
	Register *nextPC = &nextRF[30];
    Instr *cir = new Instr; //RF[31]

    //Units
    std::array<ReservationStation, RS_COUNT> RSs;

    BPB branchBuffer = BPB();
    FetchUnit fetchUnit(&branchBuffer, currentPC, nextPC, INSTR);
    DecodeUnit decodeUnit(&fetchUnit.current, &fetchUnit.next, &RSs);
    std::array<ExecutionUnit *, EXEC_COUNT> EUs;
    ReorderBuffer ROB(&EUs, nextRF, nextMEM, &RSs);
    EUs[0] = new EU::ALU(&RSs[0], &ROB);
    EUs[1] = new EU::ALU(&RSs[1], &ROB);
    EUs[2] = new EU::LSU(currentMEM, &RSs[2], &ROB);
    EUs[3] = new EU::BranchUnit(nextPC, &RSs[3], &ROB);

    RSs[0] = ReservationStation(&RSs, &ROB, currentRF, &decodeUnit.currentIssued, &decodeUnit.nextIssued, ALU, 0, RS_COUNT);
    RSs[1] = ReservationStation(&RSs, &ROB, currentRF, &decodeUnit.currentIssued, &decodeUnit.nextIssued, ALU, 1, RS_COUNT);
    RSs[2] = ReservationStation(&RSs, &ROB, currentRF, &decodeUnit.currentIssued, &decodeUnit.nextIssued, LDST, 2, RS_COUNT);
    RSs[3] = ReservationStation(&RSs, &ROB, currentRF, &decodeUnit.currentIssued, &decodeUnit.nextIssued, BRANCH, 3, RS_COUNT);

    if(argc < 2) {
        std::cout << "Error: Missing arguments. Arg1: assembly source" << std::endl;
        return 1;   
    }
    loadProgram(argv[1], INSTR);


    // while(!halt) {
    //     std::cout << "--------------------- Cycle:  " << cycles << " ----------------------" << std::endl;
    //     // writeBackUnit.tick();
    //     // memoryUnit.tick();
    //     // executionUnit.tick();
    //     // fetchUnit.before();
    //     if(cycles > 24) break;
    //     if(fetchUnit.current.size() > 0 && fetchUnit.current.front().opcode == opcode::HALT) halt = true;
    //     fetchUnit.tick();
    //     decodeUnit.tick();
    //     for(int i = 0; i < RS_COUNT; i++) RSs[i].tick();
    //     // std::cout << decodeUnit.currentIssued.size() << std::endl;
    //     // std::cout << "here2" << std::endl;
    //     cycles++;
    //     decodeUnit.update();
    //     fetchUnit.update();
    //     for(int i = 0; i < RS_COUNT; i++) RSs[i].update();
    //     // std::cout << fetchUnit.current.back().opcode;
        

    // }

    for(int i = 0; i < 1024; i++) {
        if (currentMEM[i] != 0) std::cout << currentMEM[i] << std::endl;
    }

    for(int i = 0; i < 512; i++) {
        if (INSTR[i].opcode != opcode::NOP) std::cout << (int)INSTR[i].opcode << std::endl;
    }
    std::cout << "Cycles: " << cycles << std::endl;
    return 0;
}