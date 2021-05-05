#pragma once
#include "instr.h"
#include <ReservationStation.h>
#include <ReorderBuffer.hpp>

// class ExecutionUnit {
// private: 
//     bool *halt;
//     PipelineRegister *idex;
//     PipelineRegister *exmem;

// public:
//     ExecutionUnit(bool *halt, PipelineRegister *idex, PipelineRegister *exmem);

//     int execute();
// };

const int EXEC_COUNT = 4;

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
};