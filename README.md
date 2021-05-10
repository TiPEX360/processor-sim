# Processor Simulator
## Introduction

A simulator for a superscalar, out of order, pipelined processor.

## Features

- Superscalar
- Out of order
- Branch prediction
- Tomasulo's algorithm
- Speculative execution
- Pipelined
- Non-blocking issue

## ISA

The processor supports the following instructions:

 Opcode |Operands  |  Description
------|---------------|--------------------------------------------------------------------------------------
 nop | | 
 add | Rd Rn Ri |Add the contents of ```Rn``` and ```Ri```, and store the result in ```Rd```.
 mul | Rd Rn Ri | Multiply the contents of ``` Rn ``` and ```Ri```, and store the result in ```Rd```. 
 sub | Rd Rn Ri | Subtract the contents of ``` Rn ``` and ```Ri```, and store the result in ```Rd```. 
 div | Rd Rn Ri | Perform integer division of ```Rn``` by ```Ri```, and store the result in ```Rd```.
 lsh | Rd Rn Ri | Perform left shift of ```Rn``` by ```Ri```, and store the result in ```Rd```.
 rsh | Rd Rn Ri | Perform right shift of ```Rn``` by ```Ri```, and store the result in ```Rd```.
 and | Rd Rn Ri | Perform bitwise and  of ```Rn``` and ```Ri```. Store the result in ```Rd```.
 or | Rd Rn Ri | Perform bitwise or  of ```Rn``` and ```Ri```. Store the result in ```Rd```.
 xor | Rd Rn Ri | Perform bitwise xor  of ```Rn``` and ```Ri```. Store the result in ```Rd```.
 ld | Rd Rn Ri | Load from memory address ```Rn + Ri ``` , and store in ```Rd```.
 ldc | Rd Ri | Load ```Ri``` into register ```Rd```.
 st | Rd Rn Ri | Store ```Rd``` in ```r1 + r2```.
blt | Rd Rn Ri | Branch to ```Rd``` if ```Rn < Ri```.
bz | Rd Ri | ```Rd``` if ```Ri == 0```.
bnz | Rd Ri | ```Rd``` if ```Ri != 0```.
b |   | Branch to ```Rd```.
jlt | dest | Increments program counter by ```Rd``` if ```Rn < Ri```.
jz | Rd Rn Ri | Increments program counter by ```Rd``` if ```Ri == 0```.
jnz | Rd Ri | Increments program counter by ```Rd``` if ```Ri != 0```.
j | Rd | Increments program counter by ```Rd```.
halt | | Halt execution of the processor.


Rn is always register addressed.
Ri may be register addressed or be an immediate.

## Usage

1. Write, clone, download, steal, or otherwise acquire an assembly program compatible with the ISA specified above.
2. Compile the simulator from source using
```
make
```
3. Execute the assembly program using the ```proc``` program:
```
./proc {assembly source} {scalar width} {branch prediction type (dynamic, static, fixedskip, fixedtake)} {No. EUs}
```