#pragma once
#include "instr.h"
#include <vector>
#include <queue>
// #include "ReorderBuffer.hpp"

class ReorderBuffer;

struct ROBEntry;

const int RS_SIZE = 8;


class ReservationStation {
private:
    int RSCount;
    Register *RF;
    std::vector<Instr> *issuedCurrent;
    std::vector<Instr> *issuedNext;
    std::array<ReservationStation, RS_COUNT> *RSs;
    ReorderBuffer *ROB;
public:
    int RSID;
    int nextOccupied = 0;
    int currentOccupied = 0;
    std::vector<RSEntry> currentEntries;
    std::vector<RSEntry> nextEntries;
    RSType type;
    void tick();
    void update();
    void addEntry(Instr i);
    int updateEntry(int RS, ROBEntry e);
    int removeEntry(ROBEntry e);
    RSEntry getReadyEntry();
    ReservationStation() {};
    ReservationStation(std::array<ReservationStation, RS_COUNT> *RSs, ReorderBuffer *ROB, Register *RF, std::vector<Instr> *issuedCurrent, std::vector<Instr> *issuedNext, RSType type, int RSID, int RSCount); 
};