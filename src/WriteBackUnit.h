#include "instr.h"
#include "registers.h"

class WriteBackUnit {
private:
    int *RF;
    PipelineRegister *memwb;
public:
    WriteBackUnit(int *RF, PipelineRegister *memwb);
    int wb();
};