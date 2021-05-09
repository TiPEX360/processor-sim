#pragma once
#include "instr.h"
#include "ReservationStation.h"
#include <queue>
#include "FetchUnit.h"
#include "ReorderBuffer.hpp"

struct DecodeData {
    Instr i;
};



class DecodeUnit {
private:
    std::array<ReservationStation, RS_COUNT> *RSs;
    std::deque<Instr> *currentFetched;
    std::deque<Instr> *nextFetched;
    ReorderBuffer *ROB;

    bool issueInstr(int counter, Instr n);
public:
    std::vector<Instr> currentIssued;
    std::vector<Instr> nextIssued;


    void tick();
    void update();

    DecodeUnit();
    DecodeUnit(std::deque<Instr> *currentFetched, std::deque<Instr> *nextFetched, std::array<ReservationStation, RS_COUNT> *RSs, ReorderBuffer *ROB);
};