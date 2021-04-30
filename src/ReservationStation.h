#include "instr.h"
#include <vector>

struct RSData {
    opcode opcode;
    RSID RSd;
    RSID RSn;
    RSID RSi;
    int Rd;
    int Rn;
    int Ri;
    bool busy;
    bool ready;
};

class ReservationStation {
private:
    PipelineRegister *idrs;
    PipelineRegister *rsex;
    int RSCount;
    Register *RF;
public:
    std::vector<RSData> current;
    std::vector<RSData> next;
    void tick();
    void update();
    ReservationStation(Register *RF, PipelineRegister *idrs, int RSCount);
};