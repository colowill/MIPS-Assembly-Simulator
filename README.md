<!-- omit in toc -->
# Lab 4: Basic MIPS simulator

In this lab, you'll use the C programming language to simulate the execution of MIPS assembly programs running on a MIPS microprocessor. This simulator supports arithmetic, logic, and shift operations such as `add`, `sub`, `or`, `sll`, etc. Because you won't implement advanced operations that perform control flow (e.g., loops and branching) and main memory I/O (read and write), the simulator is more akin to a calculator than a full-fledged microprocessor. However, this repo's code structure resembles that of the COMP 541 final project emulator, which supports 31 MIPS instructions. So, this repo could easily be extended to simulate a full-fledged MIPS microprocessor, but this is outside the scope of COMP 211.

Your simulator will

1. Represent MIPS assembly instructions in a C struct.
2. Read and write values in simulated general-purpose registers (an `int32_t` array of length 32).
3. Emulate the operations done by MIPS instructions and update the program counter (PC).

The main objectives of this lab are for you to

1. Become familiar with basic MIPS microprocessor design.
    - Given an instruction in 32-bit format, determine the instruction's type, name, and fields.
    - Emulate register updates performed by MIPS instructions.
2. Enhance your knowledge of C's struct, pointers, bitwise operations (logic and arithmetic), and memory operations.
3. Introduce and apply new C concepts: enum, bit field, union, and function pointer. All have practical applications in this repo.
