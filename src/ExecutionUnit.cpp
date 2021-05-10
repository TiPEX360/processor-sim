#include "ExecutionUnit.h"

void ExecutionUnit::update() {
    currentOut = nextOut;
}

void ExecutionUnit::flush() {
    RSEntry nullEntry;
    nullEntry.opcode = opcode::NOP;
    nullEntry.executing = true;
    processing = nullEntry;
    nextOut = {-1, InstrType::NOP, -1, -1, false, 0};
    duration = 1;
    progress = 0;
}