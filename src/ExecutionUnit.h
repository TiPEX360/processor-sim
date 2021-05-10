#pragma once
#include "instr.h"
#include "ReservationStation.h"


class ExecutionUnit {
protected: 
    ReservationStation *RS;
    ReorderBuffer *ROB;
    RSEntry processing;
    int progress;
    int duration;
    // ROBEntry out;
public:
    // ExecutionUnit(bool *halt, PipelineRegister *idex, PipelineRegister *exmem);
    ROBEntry currentOut;
    ROBEntry nextOut;
    // int execute();
    virtual void tick() = 0;
    void update();
    void flush();
};