#pragma once
#include "instr.h"
#include <vector>
#include <queue>


const int RS_SIZE = 8;

enum RSType {ALU, LDST, BRANCH};

struct RSEntry {
    opcode opcode;
    RSID RSd;
    RSID RSn;
    RSID RSi;
    int Rd;
    int Rn;
    int Ri;
    bool busy;
    bool ready;
};

class ReservationStation {
private:
    PipelineRegister *idrs;
    PipelineRegister *rsex;
    int RSCount;
    Register *RF;
    std::vector<Instr> *issuedCurrent;
    std::vector<Instr> *issuedNext;
public:
    int RSID;
    std::vector<RSEntry> currentEntries;
    std::vector<RSEntry> nextEntries;
    RSType type;
    void tick();
    void update();
    ReservationStation() {};
    ReservationStation(Register *RF, std::vector<Instr> *issuedCurrent, std::vector<Instr> *issuedNext, RSType type, int RSID, int RSCount); 
};