#include "ReorderBuffer.hpp"

ROBType ReorderBuffer::addEntry(Instr i) {

}

void ReorderBuffer::tick() {

}

void ReorderBuffer::update() {
    currentROB = nextROB;
}

ReorderBuffer::ReorderBuffer(Register *RF, int32_t *MEM, std::vector<ReservationStation> *RSs) {
    ReorderBuffer::RF = RF;
    ReorderBuffer::MEM = MEM;
    ReorderBuffer::RSs = RSs;
}

ReorderBuffer::ReorderBuffer()  {};