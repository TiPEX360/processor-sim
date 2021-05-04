#pragma once
#include <ReservationStation.h>
#include <ReorderBuffer.hpp>
#include <ExecutionUnit.h>



class LSU : public ExecutionUnit {
private:
    uint32_t *MEM;
public:
    virtual void tick();
    void update();

    LSU(uint32_t *MEM, ReservationStation *RS, ReorderBuffer *ROB);
    LSU();
};