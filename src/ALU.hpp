#pragma once
#include <ReservationStation.h>
#include <ReorderBuffer.hpp>
#include <ExecutionUnit.h>


namespace EU {
    class ALU : public ExecutionUnit {
    public:
        virtual void tick();
        void update();

        ALU(ReservationStation *RS, ReorderBuffer *ROB);
        ALU();
    };
}