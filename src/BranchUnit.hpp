#pragma once
#include "ReservationStation.h"
#include "ReorderBuffer.hpp"
#include "ExecutionUnit.h"

class BranchUnit : public ExecutionUnit {
private:
    Register *pc;
    ReservationStation *RS;
    ReorderBuffer *ROB;
public:
    virtual void tick();
    void update();
    BranchUnit(Register *pc, ReservationStation *RS, ReorderBuffer *ROB);
    BranchUnit();    
}