#pragma once
#include "instr.h"
#include <vector>
#include <queue>

class ReorderBuffer;

struct ROBEntry;

const int RS_SIZE = 8;

class ReservationStation {
private:
    int RSCount;
    Register *RF;
    std::vector<Instr> *issuedCurrent;
    std::vector<Instr> *issuedNext;
    std::vector<ReservationStation> *RSs;
    ReorderBuffer *ROB;
public:
    BundleWriteLog *BWL;
    int RSID;
    std::vector<RSEntry> currentEntries;
    std::vector<RSEntry> nextEntries;
    RSType type;
    void tick();
    void update();
    void addEntry(int counter, Instr i);
    void updateEntry(int RS, ROBEntry e);
    int removeEntry(ROBEntry e);
    RSEntry getReadyEntry();
    ReservationStation() {};
    ReservationStation(BundleWriteLog *BWL, std::vector<ReservationStation> *RSs, ReorderBuffer *ROB, Register *RF, std::vector<Instr> *issuedCurrent, std::vector<Instr> *issuedNext, RSType type, int RSID, int RSCount); 
};