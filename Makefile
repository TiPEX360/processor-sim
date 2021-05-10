proc: ./src/proc.cpp
	clang++ ./src/proc.cpp ./src/DecodeUnit.cpp ./src/ExecutionUnit.cpp ./src/BranchUnit.cpp ./src/ALU.cpp ./src/LSU.cpp ./src/ReservationStation.cpp ./src/ReorderBuffer.cpp ./src/BPB.cpp ./src/FetchUnit.cpp -o ./bin/proc
