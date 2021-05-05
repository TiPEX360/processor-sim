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
    // PipelineRegister *idrs;
    // PipelineRegister *ifid;
    std::array<ReservationStation, RS_COUNT> *RSs;
    std::queue<Instr> *currentFetched;
    std::queue<Instr> *nextFetched;
    ReorderBuffer *ROB;
public:
    // DecodeData current;
    // DecodeData next;

    std::vector<Instr> currentIssued;
    std::vector<Instr> nextIssued;
    // IssuedInstr n


    void tick();
    void update();

    DecodeUnit();
    DecodeUnit(std::queue<Instr> *currentFetched, std::queue<Instr> *nextFetched, std::array<ReservationStation, RS_COUNT> *RSs);
};