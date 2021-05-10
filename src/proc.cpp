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
#include "ReservationStation.h"
#include "ReorderBuffer.hpp"
#include "BPB.hpp"
#include <time.h>



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
        instr.branchTaken = 0;
        instr.RSID = 0;

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
            else if(tokens[0].compare("bz") == 0) instr.opcode = opcode::BZ;
            else if(tokens[0].compare("bnz") == 0) instr.opcode = opcode::BNZ;
            else if(tokens[0].compare("b") == 0) instr.opcode = opcode::B;
            else if(tokens[0].compare("j") == 0) instr.opcode = opcode::J;  
            else if(tokens[0].compare("jlt") == 0) instr.opcode = opcode::JLT;
            else if(tokens[0].compare("jz") == 0) instr.opcode = opcode::JZ;
            else if(tokens[0].compare("jnz") == 0) instr.opcode = opcode::JNZ;
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

            INSTR[instrIndex] = instr;
            instrIndex++;

        }
    }
    in.close();
}

extern int instrCount;

int SCALE_WIDTH;
int RS_COUNT;
int EXEC_COUNT;
BPType PREDICTIONTYPE;
int main(int argc, char *argv[]) {
    if(argc < 2) {
        std::cout << "Error: Missing arguments. Arg1: assembly source path. Arg2: Scale width (int). Arg3: Branch prediction method ({static, dynamic, fixedtake, fixedskip}). Arg4: EU Count (int > 2)." << std::endl;
        return 1;   
    }
    else if(argc == 2) {    
        SCALE_WIDTH = 4;
        PREDICTIONTYPE = BPType::DYNAMIC;
        RS_COUNT = 4;
        EXEC_COUNT = 4;
    }
    else if(argc == 3) {
        SCALE_WIDTH = atoi(argv[2]);
        PREDICTIONTYPE = BPType::DYNAMIC;
        RS_COUNT = 4;
        EXEC_COUNT = 4;
    }
    else if(argc == 4) {
        SCALE_WIDTH = atoi(argv[2]);
        RS_COUNT = 4;
        EXEC_COUNT = 4;
        std::string in = argv[3];
        if(in.compare("static") == 0) PREDICTIONTYPE = BPType::STATIC;
        else if(in.compare("dynamic") == 0) {
            PREDICTIONTYPE = BPType::DYNAMIC;
        }
        else if(in.compare("fixedtake") == 0) PREDICTIONTYPE = BPType::FIXEDTAKE;
        else if(in.compare("fixedskip") == 0) PREDICTIONTYPE = BPType::FIXEDSKIP;
        else {
            std::cout << "ERROR: Invalid branch prediction method. Valid arguments: {static, dynamic, fixedtake, fixedskip}" << std::endl;
            return 1;
        }
    }
    else if(argc == 5) {
        SCALE_WIDTH = atoi(argv[2]);
        if(atoi(argv[4]) < 3) {
            std::cout << "ERROR: Invalid number of excution units. Valid arguments: (int > 2)" << std::endl;
            return 1;
        }
        RS_COUNT = atoi(argv[4]);
        EXEC_COUNT = atoi(argv[4]);
        std::string in = argv[3];
        if(in.compare("static") == 0) PREDICTIONTYPE = BPType::STATIC;
        else if(in.compare("dynamic") == 0) {
            PREDICTIONTYPE = BPType::DYNAMIC;
        }
        else if(in.compare("fixedtake") == 0) PREDICTIONTYPE = BPType::FIXEDTAKE;
        else if(in.compare("fixedskip") == 0) PREDICTIONTYPE = BPType::FIXEDSKIP;
        else {
            std::cout << "ERROR: Invalid branch prediction method. Valid arguments: {static, dynamic, fixedtake, fixedskip}" << std::endl;
            return 1;
        }
    }
    else SCALE_WIDTH = 4;

    //Buffers
    BundleWriteLog BWL[SCALE_WIDTH];
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
    std::vector<ReservationStation> RSs;
    RSs.reserve(RS_COUNT);

    BPB branchBuffer = BPB();
    FetchUnit fetchUnit(&branchBuffer, currentPC, nextPC, INSTR);
    std::vector<ExecutionUnit *> EUs;
    EUs.reserve(EXEC_COUNT);
    bool halt = false; //KNOWN TO BE ROOT OF ALL PROBLEMS
    ReorderBuffer ROB(&branchBuffer, &fetchUnit.currentFetched, &fetchUnit.nextFetched, &halt, &EUs, nextRF, nextMEM, &RSs);
    DecodeUnit decodeUnit(&fetchUnit.currentFetched, &fetchUnit.nextFetched, &RSs, &ROB);

    for(int i = 0; i < EXEC_COUNT - 2; i++) {
        EUs.push_back(new EU::ALU(&RSs[i], &ROB));
        RSs.push_back(ReservationStation(BWL, &RSs, &ROB, currentRF, &decodeUnit.currentIssued, &decodeUnit.nextIssued, RSType::ALU, i, RS_COUNT));
    }
    std::cout << "here" << std::endl;
    EUs.push_back(new EU::LSU(currentMEM, &RSs[EXEC_COUNT - 2], &ROB));
    EUs.push_back(new EU::BranchUnit(nextPC, &RSs[EXEC_COUNT - 1], &ROB));
    RSs.push_back(ReservationStation(BWL, &RSs, &ROB, currentRF, &decodeUnit.currentIssued, &decodeUnit.nextIssued, RSType::LDST, EXEC_COUNT - 2, RS_COUNT));
    RSs.push_back(ReservationStation(BWL, &RSs, &ROB, currentRF, &decodeUnit.currentIssued, &decodeUnit.nextIssued, RSType::BRANCH, EXEC_COUNT - 1, RS_COUNT));
    // EUs[0] = new EU::ALU(&RSs[0], &ROB);
    // EUs[1] = new EU::ALU(&RSs[1], &ROB);
    // EUs[2] = new EU::LSU(currentMEM, &RSs[2], &ROB);
    // EUs[3] = new EU::BranchUnit(nextPC, &RSs[3], &ROB);

    // RSs[0] = ReservationStation(BWL, &RSs, &ROB, currentRF, &decodeUnit.currentIssued, &decodeUnit.nextIssued, RSType::ALU, 0, RS_COUNT);
    // RSs[1] = ReservationStation(BWL, &RSs, &ROB, currentRF, &decodeUnit.currentIssued, &decodeUnit.nextIssued, RSType::ALU, 1, RS_COUNT);
    // RSs[2] = ReservationStation(BWL, &RSs, &ROB, currentRF, &decodeUnit.currentIssued, &decodeUnit.nextIssued, RSType::LDST, 2, RS_COUNT);
    // RSs[3] = ReservationStation(BWL, &RSs, &ROB, currentRF, &decodeUnit.currentIssued, &decodeUnit.nextIssued, RSType::BRANCH, 3, RS_COUNT);


    loadProgram(argv[1], INSTR);

    int cycles = 0;
    int time = clock();
    int deltaTime = clock() - time;
    while(!halt) {
        deltaTime = clock() - deltaTime;
        // if(deltaTime % 10 == 0) std::cout << "-";
        cycles = cycles + 1;

        //Tick
        fetchUnit.tick();
        for(int i = 0; i < SCALE_WIDTH; i++) BWL[i] = {-1, -1};
        decodeUnit.tick();
        for(int i = 0; i < EXEC_COUNT; i++) {
            EUs[i]->tick();
        }
        ROB.tick();

        //Update State
        decodeUnit.update();
        fetchUnit.update();
        for(int i = 0; i < 32; i++) currentRF[i] = nextRF[i];
        for(int i = 0; i < 1024; i++) currentMEM[i] = nextMEM[i];
        for(int i = 0; i < RS_COUNT; i++) RSs[i].update();
        ROB.update();
        for(int i = 0; i < EXEC_COUNT; i++) {
            EUs[i]->update(); 
        }
        // std::cout << fetchUnit.current.back().opcode;
        

    }

    for(int i = 0; i < 1024; i++) {
        if(currentMEM[i] != 0) std::cout << "MEM " << i << ": " << currentMEM[i] << std::endl;
    }


    std::cout << "----------------------------------" << std::endl;
    std::cout << "Cycles: " << cycles << std::endl;
    std::cout << "EU Count: " << EXEC_COUNT << std::endl;
    std::cout << "RS Count: " << RS_COUNT << std::endl;
    std::cout << "Scale width: " << SCALE_WIDTH << std::endl;
    std::cout << "Instructions finished " << instrCount << std::endl;
    std::cout << "IPC: " <<  (float)instrCount  / (float) cycles << std::endl;
    
    return 0;
}