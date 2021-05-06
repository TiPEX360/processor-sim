#pragma once
#include <ReservationStation.h>
#include <ReorderBuffer.hpp>
#include <ExecutionUnit.h>


namespace EU {
    class LSU : public ExecutionUnit {
    private:
        int32_t *currentMEM;
        ReservationStation *RS;
        ReorderBuffer *ROB;
    public:
        virtual void tick();
        void update();

        LSU(int32_t *currentMEM, ReservationStation *RS, ReorderBuffer *ROB);
        LSU();
    };
}