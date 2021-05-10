#pragma once
#include <vector>
#include <queue>
#include "instr.h"
#include "BPB.hpp"

class ExecutionUnit;
class ReservationStation;

typedef int ROBID;

const int ROB_MAX = 128;


class ReorderBuffer {
private:
    Register *nextRF;
    int32_t *nextMEM;
    std::vector<ReservationStation> *RSs;
    std::vector<ExecutionUnit *> *EUs;
    std::deque<Instr> *nextFetched;
    std::deque<Instr> *currentFetched;
    
    BPB *branchBuffer;
    bool *halt;
public:
    std::vector<ROBEntry> currentROB;
    std::vector<ROBEntry> nextROB;

    int updateEntry(int index, ROBEntry e);
    ROBEntry addEntry(RSEntry RSe, RSID RSID, int branchTaken);
    void flush(ROBEntry branchEntry);
    RSID findRSIDByROBEntry(ROBEntry e);
    void tick();
    void update();
    ReorderBuffer() {};
    ReorderBuffer(BPB *branchBuffer, std::deque<Instr> *currentFetched, std::deque<Instr> *nextFetched, bool *halt, std::vector<ExecutionUnit *> *EUs, Register *nextRF, int32_t *nextMEM, std::vector<ReservationStation> *RSs);
};