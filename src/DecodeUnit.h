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
    std::array<std::queue<Instr>, 4> *currentFetched;
    std::array<std::queue<Instr>, 4> *nextFetched;
    ReorderBuffer *ROB;

    bool issueInstr(int counter, Instr n);
public:
    std::vector<Instr> currentIssued;
    std::vector<Instr> nextIssued;


    void tick();
    void update();

    DecodeUnit();
    DecodeUnit(std::array<std::queue<Instr>, 4> *currentFetched, std::array<std::queue<Instr>, 4> *nextFetched, std::array<ReservationStation, RS_COUNT> *RSs, ReorderBuffer *ROB);
};