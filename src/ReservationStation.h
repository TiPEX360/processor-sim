#pragma once
#include "instr.h"
#include <vector>
#include <queue>
#include "ReorderBuffer.hpp"


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
    ROBID ROBId;
};

class ReservationStation {
private:
    int RSCount;
    Register *RF;
    std::vector<Instr> *issuedCurrent;
    std::vector<Instr> *issuedNext;
    ReorderBuffer *ROB;
public:
    int RSID;
    std::vector<RSEntry> currentEntries;
    std::vector<RSEntry> nextEntries;
    RSType type;
    void tick();
    void update();
    ReservationStation() {};
    ReservationStation(ReorderBuffer *ROB, Register *RF, std::vector<Instr> *issuedCurrent, std::vector<Instr> *issuedNext, RSType type, int RSID, int RSCount); 
};