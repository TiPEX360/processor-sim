# Processor Simulator
## Introduction

A simulator for a simple, scalar, non-pipelined processor.

## ISA

The processor supports the following instructions:

 Opcode |Operands  |  Description
---|---|---
 add | dest r1 r2 |Add the contents of ```r1``` and ```r2```, and store the result in ```dest``` | 
 addi| dest r1 #1| Add the contents of ``` r1 ``` and an immediate ```#1```, and store the result in ```dest```. 
 mul | dest r1 r2 | Multiplythe contents of ``` r1 ``` and an immediate ```#1```, and store the result in ```dest```. 
 sub | dest r1 r2 | Subtractthe contents of ``` r1 ``` and an immediate ```#1```, and store the result in ```dest```. 
 ldm | dest a1 a2 | Load from memory address ```r1 + r2 ``` , and store in ```dest```.
 ldc | dest #1 | Store the ```#1``` immediate in ```dest``` register.
stm | r a1 a2 | Store the contents of ```r``` in the memory address ```r1 + r2```.
blt | dest cmp1 cmp2 | Branch to ```dest``` if ```cmp1 < cmp2```.
bnz | dest cmp1  | Branch to ```dest``` if ```cmp1 < 0```.
br | dest | Branch to ```dest```.
halt | | Halt execution of the processor.


## Usage

1. Write, download, steal, or otherwise acquire an assembly program compatible with the ISA specified above.
2. Create a binary file using the ```assembler``` program provided:
```
 ./assembler {input} {output}
```
3. Execute the binary file using the ```proc``` program:
```
./proc {binary}
```