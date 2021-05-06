#pragma once
#include "ReservationStation.h"
#include "ReorderBuffer.hpp"
#include "ExecutionUnit.h"

namespace EU {
    class BranchUnit : public ExecutionUnit {
    private:
        Register *nextPC;
        ReservationStation *RS;
        ReorderBuffer *ROB;
    public:
        virtual void tick();
        void update();
        BranchUnit(Register *nextPC, ReservationStation *RS, ReorderBuffer *ROB);
        BranchUnit();    
    };
}