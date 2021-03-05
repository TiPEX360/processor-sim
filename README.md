# Processor Simulator
## Introduction

A simulator for a simple, scalar, non-pipelined processor.

## ISA

The processor supports the following instructions:

 Opcode |Operands  |  Description
---|---|---
 add | dest r1 r2 |Add the contents of ```r1``` and ```r2```, and store the result in ```dest```.
 addc| dest r1 #1| Add the contents of ``` r1 ``` and an immediate ```#1```, and store the result in ```dest```. 
 mul | dest r1 r2 | Multiply the contents of ``` r1 ``` and ```r2```, and store the result in ```dest```. 
 sub | dest r1 r2 | Subtract the contents of ``` r1 ``` and ```r2```, and store the result in ```dest```. 
 div | dest r1 r2 | Perform integer division of ```r1``` by ```r2```, and store the result in ```dest```.
 lsh | dest r1 r2 | Perform left shift of ```r1``` by ```r2```, and store the result in ```dest```.
 rsh | dest r1 r2 | Perform right shift of ```r1``` by ```r2```, and store the result in ```dest```.
 and | dest r1 r2 | Perform bitwise and  of ```r1``` and ```r2```. Store the result in ```dest```.
 or | dest r1 r2 | Perform bitwise or  of ```r1``` and ```r2```. Store the result in ```dest```.
 xor | dest r1 r2 | Perform bitwise xor  of ```r1``` and ```r2```. Store the result in ```dest```.
 ldm | dest src1 src2 | Load from memory address ```src1 + src2 ``` , and store in ```dest```.
 ldmc | dest src #offset | Load from memory address stored in register ```src``` offset by immediate ```#offset```. Store in ```dest```.
 ldc | dest #1 | Store the ```#1``` immediate in ```dest``` register.
stm | src dest1 dest2 | Store the contents of ```src``` in the memory address ```dest1 + dest2```.
stmc | #src dest1 #dest2 | Like stm but ```#dest2``` is an immediate.
sto | #dest #src1 #src2 | Like stm but all operands are immediates.
blt | dest cmp1 cmp2 | Branch to ```dest``` if ```cmp1 < cmp2```.
bnz | dest cmp1  | Branch to ```dest``` if ```cmp1 != 0```.
br | dest | Branch to ```dest```.
jlt | #inc cmp1 cmp2 | Increments program counter by ```#inc``` if ```cmp1 < cmp2```.
jnz | #inc cmp1 | Increments program counter by ```#inc``` if ```cmp1 != 0```.
j | #inc | Increments program counter by ```#inc```.
cmp | dest a1 a2 | Compares ```a1``` to ```a2``` and stores the result in ```dest```. ```0 => (a1 == a2)```. ```-1 => (a1 < a2)```. ```1 => (a1 > a2)```.
halt | | Halt execution of the processor.

## Usage

1. Write, clone, download, steal, or otherwise acquire an assembly program compatible with the ISA specified above.
2. Create a binary file using the ```assembler``` program provided:
```
 ./assembler {input} {output}
```
3. Execute the binary file using the ```proc``` program:
```
./proc {binary}
```