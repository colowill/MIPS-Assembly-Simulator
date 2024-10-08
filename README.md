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

<details open>
    <summary>Contents</summary>

- [Background reading](#background-reading)
- [Repo structure](#repo-structure)
- [Input/output](#inputoutput)
    - [addi.asm (input)](#addiasm-input)
    - [addi.hex (input)](#addihex-input)
    - [addi.log (output)](#addilog-output)
- [Understand given code](#understand-given-code)
    - [Enum](#enum)
    - [r\_fields and i\_fields structs](#r_fields-and-i_fields-structs)
    - [Union](#union)
    - [instruction struct](#instruction-struct)
        - [Function pointer](#function-pointer)
        - [Execute instruction](#execute-instruction)
- [Check for memory leaks with Valgrind](#check-for-memory-leaks-with-valgrind)
    - [How to debug memory leaks](#how-to-debug-memory-leaks)
- [Ignored nuances](#ignored-nuances)
- [Submit your assignment](#submit-your-assignment)

</details>

## Background reading

1. [GNU Compiler Collection (GCC): Compile, Assemble, Machine Language](https://uncch.instructure.com/users/9947/files/5281339?verifier=TFMoH6zJsSK35Vd4cCoxOXtTsHnUk9duDuBXABCj&wrap=1A) (starting on pg. 8) ([videos](https://uncch.hosted.panopto.com/Panopto/Pages/Sessions/List.aspx?folderID=bb9b671d-49d3-4c1d-92ee-b1d001666678))
2. [MIPS Cheat Sheet](https://uncch.instructure.com/users/9947/files/4534610?verifier=0lJburrtzSqIT791v3YyAlhY3ZBq0MykyBPR1nY6&wrap=1)
    - Very useful for this lab
3. [Integers (The GNU C Library)](https://www.gnu.org/software/libc/manual/html_node/Integers.html)
    - In this lab, we use types such as `uint8_t`, `uint16_t`, `uint32_t`, `int16_t`, and `int32_t`

## Repo structure

Since this repo has more files than usual, here's a brief description of each one. If you shouldn't modify a file (because it is already complete), the file specifies it in a comment at the top.

```text
.
├── constants.h - Important constants you should use while coding
├── data - Contains data files for testing MIPS simulator
│   ├── addi.asm - Source assembly code
│   ├── addi.hex - Assembly code in hex form parsable by the simulator
│   ├── addi.log - Expected output of ./main data/addi.hex
│   ... omitted
├── instructions.c - Write all code here
├── instructions.h - Important documentation for instructions.c and function prototypes
├── main.c - Runs the simulator
├── Makefile - Different from previous Makefiles because it generates two executables, "main" and "tests"
├── README.md
├── tests.cpp - Test cases
├── tests.hpp
├── types.h - Important custom types you must understand while coding
├── utils.c - Not important, helper functions for the simulator
└── utils.h
```

You won't have to modify any file except `instructions.c`, but you have to understand the code and comments in files marked "important".

## Input/output

To show you what the simulator does at a high level, example inputs and outputs are in [data/](data). Specifically, consider the three files `addi.asm`, `addi.hex`, and `addi.log`.

### addi.asm (input)

```asm
addi $8, $0, 1
addi $9, $0, 2
```

This file contains simple MIPS assembly code similar to what you've seen in [lecture slides](#background-reading).

### addi.hex (input)

```text
20080001
20090002
```

This file contains the same MIPS assembly code as `addi.asm` but in hexadecimal format. That is, each line is a single MIPS instruction represented as a string of 8 hexits or 32 bits. Our simulator requires this hex format because it cannot understand strings such as `addi $8, $0, 1`. You should know how to do this translation manually, but the process is tedious.

If necessary for testing (but probably not because we provide unit tests in `tests.cpp`), you can write MIPS assembly code and translate it to hex automatically using Mars, a MIPS IDE and simulator. You can see steps for downloading and using Mars by compiling with `make` and then running `./main -m`. Or, if your code doesn't compile, see the comments at the top of `main.c`.

### addi.log (output)

```text
| Name |    Value   |
---------------------
|   PC |          8 |
|  $ 0 |          0 |
|  $ 1 |          0 |
|  $ 2 |          0 |
|  $ 3 |          0 |
|  $ 4 |          0 |
|  $ 5 |          0 |
|  $ 6 |          0 |
|  $ 7 |          0 |
|  $ 8 |          1 |
|  $ 9 |          2 |
|  $10 |          0 |
|  $11 |          0 |
|  $12 |          0 |
|  $13 |          0 |
|  $14 |          0 |
|  $15 |          0 |
|  $16 |          0 |
|  $17 |          0 |
|  $18 |          0 |
|  $19 |          0 |
|  $20 |          0 |
|  $21 |          0 |
|  $22 |          0 |
|  $23 |          0 |
|  $24 |          0 |
|  $25 |          0 |
|  $26 |          0 |
|  $27 |          0 |
|  $28 |          0 |
|  $29 |          0 |
|  $30 |          0 |
|  $31 |          0 |
```

This file was generated by running `./main data/addi.hex > data/addi.log`. You can confirm that both assembly instructions were run because the values in registers `$8` and `$9` are 1 and 2, as expected.

Your `main` executable won't generate correct output until you're completely done with the lab. But, `tests.cpp` unit tests each helper function you'll write so that you'll get immediate feedback as you complete each helper function.

## Understand given code

Before you start coding, read the code and comments in files marked "important" [above](#repo-structure).

In particular, the rest of this section explains [types.h](types.h) in detail and shows several concrete examples because it contains custom types we've defined that you'll have to use. As with any project, if you understand the types, the code that you'll have to write should become much more clear or maybe even obvious.

Also, [types.h](types.h) uses C features you probably haven't seen yet, but we'll explain these. The point of using these features is that they're good practice and make this repo's code more clean.

### Enum

At the top of [types.h](types.h), there are two `enum` definitions.

```c
// @note For info about enum, see README.md and/or
// https://www.w3schools.com/c/c_enums.php
typedef enum { R_TYPE, I_TYPE } instruction_type;

typedef enum {
  SLL,
  SRA,
  ADD,
  SUB,
  AND,
  OR,
  NOR,
  ADDI,
  ANDI,
  ORI
} instruction_name;
```

We use these as return values for `determine_instruction_type` and `determine_instruction_name`. For example, the return type of `determine_instruction_type` is `instruction_type`, so in that function, you can simply write `return R_TYPE`. Comparisons are done with `==`. For example, you could write `if (determine_instruction_type(instruct) == R_TYPE)`.

Some alternatives (if you were not familiar with `enum`) would have been to return `bool`, `int`, or `char*`. Suppose we had chosen `char*`, e.g. `return "R_TYPE"`. In this context, this would be bad practice for several reasons.

1. Strings are tricky to work with, as you have seen in previous labs.
2. If the return type is string, the return value must be `malloc`ed.
3. If we can **enum**erate all possible return values, we don't need to return a string, which can hold any sequence of characters and could even be `NULL`. `enum` suffices.
4. It is easier to check the value of an `enum` than it is to check the value of a string. For example, you can use a `switch` statement for `enum` but not string.
5. Because an `enum` can be only one of a finite set of values, you get autocomplete features, unlike string.

### r_fields and i_fields structs

```c
// @note The : 5 is a bit field that tells the compiler a specific number
// of bits to use for storing each struct member (may save space)
typedef struct {
    uint8_t rs : 5;
    uint8_t rt : 5;
    uint8_t rd : 5;
    uint8_t shamt : 5;
} r_fields;

// @note The : 5 and : 16 are bit fields that tell the compiler a specific
// number of bits to use for storing each struct member (may save space)
typedef struct {
    uint8_t rs : 5;
    uint8_t rt : 5;
    int16_t immediate : 16;
} i_fields;
```

These two structs represent the fields of R-type and I-type instructions, respectively. For example, consider the I-type instruction `addi $8, $0, 1`. For this instruction, `rt` is 8, `rs` is 0, and `immediate` is 1 (see the [MIPS Cheat Sheet](https://uncch.instructure.com/users/9947/files/4534610?verifier=0lJburrtzSqIT791v3YyAlhY3ZBq0MykyBPR1nY6&wrap=1) and Ctrl/Cmd+F for "addi"). This information would be stored in an `i_fields` struct.

However, recall from [earlier](#addihex-input) that the simulator's input is MIPS instructions in hex format. So, in `create_fields`, the parameter is not a string, such as `"addi $8, $0, 1"`. Rather, the parameter is the 32-bit representation of an instruction (`uint32_t`). For example, `addi $8, $0, 1` has the 32-bit representation `0b00100000000010000000000000000001`.

To parse instructions in 32-bit form, see the top left of the [MIPS Cheat Sheet](https://uncch.instructure.com/users/9947/files/4534610?verifier=0lJburrtzSqIT791v3YyAlhY3ZBq0MykyBPR1nY6&wrap=1):

<p align="center">
    <img src="https://i.imgur.com/umTYoZG.png">
    <br>
    <em>MIPS instruction format</em>
</p>

First, determine whether the instruction is R-type or I-type. If it's R-type, use `bit_select` to extract the bits representing `rs`, `rt`, `rd`, and `shamt`, and do something similar for I-type. Then, return these values in a `r_fields` or `i_fields` struct. For more details, see the docstring for `create_fields`.

### Union

`create_fields` actually returns `fields`, not `r_fields` or `i_fields`. `fields` is a `union` of the types `r_fields` and `i_fields`, as defined below.

```c
// @note For info about union, see README.md and/or
// https://www.tutorialspoint.com/cprogramming/c_unions.htm
typedef union {
  r_fields r;
  i_fields i;
} fields;
```

The idea is similar to union types in other languages. For example, in Python, you could use a union type annotation like so:

```python
def add(x: int | float, y: int | float) -> int | float:
    return x + y
```

We define `fields` to be the union of `r_fields` and `i_fields` because if we did not, we would have to define two separate functions `create_r_fields` and `create_i_fields` that return `r_fields` and `i_fields`, respectively, rather than just one `create_fields` that returns `fields`. This would propagate in that we would also have to define two separate functions for `create_instruction` and `execute_instruction` and two separate types for `instruction`.

To understand the core idea of `union`, see the following example:

```c
#include <stdio.h>

typedef union {
    int i;
    double d;
} number;

int main() {
    printf("sizeof(number): %ld\n",
           sizeof(number));  // max(sizeof(int), sizeof(double))
    printf("sizeof(int): %ld\n", sizeof(int));
    printf("sizeof(double): %ld\n", sizeof(double));
    return 0;
}
```

```text
sizeof(number): 8
sizeof(int): 4
sizeof(double): 8
```

We define the type `number` to be **either** `int` or `double`. To enforce this, we use the `union` type. Because `number` is a `union`, it can store **either** an `int` or a `double`, so `sizeof(number)` is the size of the largest member (`double`), *not* the sum of the member sizes. This is in contrast to a `struct`, where the size is the sum of the members' sizes (plus possible padding).

In a `union`, all members share the same memory region. In particular, `int i` and `double d` share the same memory region in this example. So, only one member (`i` or `d`) can contain a valid value at any given time. For example,

```c
#include <stdio.h>

typedef union {
    int i;
    double d;
} number;

int main() {
    number n;
    n.i = 1;
    printf("%d\n", n.i);
    n.d = 1.0;
    printf("%f\n", n.d);
    // Junk value
    printf("%d\n", n.i);
    return 0;
}
```

```text
1
1.000000
0
```

If you understand the idea behind a union, this syntax and behavior should make sense.

You can see another example of `union` in some given code [below](#function-pointer) (first and second line of `addi` function body). If you want to learn more about `union`, see the link in the docstring for `fields` (given above).

### instruction struct

```c
/**
 * Represents a MIPS instruction
 *
 * A MIPS instruction is fully defined by its fields (stored in struct member
 * _fields) and its operation. Example operations - given the values in two
 * registers, does the instruction add them, subtract them, bitwise or them,
 * etc.?
 *
 * Operation code is put into a function (e.g., functions add, sub, or_op, etc.,
 * in instructions.h)
 * We assign the function's address to this struct's member
 * named execute, which is a function pointer
 *
 * @note For more details regarding the above, see README and/or
 * https://www.tutorialspoint.com/function-pointer-in-c
 */
typedef struct {
    fields _fields;
    // For info about this function signature, see comments at the bottom of
    // instructions.h
    void (*execute)(fields fields, int32_t* registers, uint32_t* pc);
} instruction;
```

To represent a MIPS assembly instruction, we define a new type `instruction`, and you'll initialize this struct in the function `create_instruction`.

Clearly, this type must store information about an instruction's fields. For example, to represent `addi $8, $0, 1`, we must store the information `$8, $0, 1` in an `i_fields` struct, as shown [above](#r_fields-and-i_fields-structs). Thus, one member of the `instruction` struct is `_fields`, which has type `fields`.

However, storing `$8, $0, 1` is not enough. To be a complete representation, the `instruction` struct must also store information about the operation that `addi` performs. That is, `addi` adds the value in register `rs` (here, value in register `$0`) to `immediate` (here, 1) and stores that in register `rt` (here, register `$8`). To generalize, the operation that `addi` performs is represented by the code `registers[i_fields.rt] = registers[i_fields.rs] + i_fields.immediate;`. So, when creating an `instruction`, we should somehow store a line of code in the struct. How is this possible?

#### Function pointer

The solution is to first put the above code in a function, like so:

```c
void addi(fields fields, int32_t* registers, uint32_t* pc) {
    i_fields i_fields = fields.i;
    registers[i_fields.rt] = registers[i_fields.rs] + i_fields.immediate;
    // pc means program counter, you may ignore this for now
    *pc = *pc + WORD_SIZE;
}
```

Then, suppose we want to create an `instruction` struct to represent `addi $8, $0, 1`. We could use the following syntax to assign the address of the `addi` function to the struct member named `execute`:

```c
instruction* rv = (instruction*)malloc(sizeof(instruction));
rv->execute = addi; // rv now stores the information that it performs the addi operation
```

This syntax works because, as shown above, the `instruction` struct member named `execute` is a function pointer. Specifically, `execute` is defined as storing the address of a function that returns `void` and has 3 parameters with types `fields`, `int32_t*`, and `uint32_t*`. The function `addi` satisfies these requirements, so we can assign the address of `addi` to `execute`.

This is the same process as normal variable assignment, which you're already familiar with. Specifically, the struct member `execute` has a specific type, and we can assign it a particular value only if that value has the same type. The only new concepts here are

1. A function has a type, which is defined by its return type and parameter type(s). For example, see the declarations of `execute` and `addi` above.
2. A function has an address, which can be stored in a pointer variable.

For example,

```c
#include <stdio.h>

int main(void) {
    // Type of variable func_ptr matches that of main function
    int (*func_ptr)(void) = main;
    printf("%p\n", func_ptr);
    return 0;
}
```

```text
0xaaaabe030754
```

#### Execute instruction

Lastly, to execute an instruction, we use one simple line of code:

```c
// This is given to you, don't edit
void execute_instruction(instruction* instruct, int32_t* registers,
                         uint32_t* pc) {
    instruct->execute(instruct->_fields, registers, pc);
}
```

Given a correctly initialized `instruction* instruct`, we use `instruct->execute` to access `instruct`'s `execute` member. Because `execute` points to a function, typing parentheses after it calls that function with the specified arguments.

It's possible for `execute_instruction`'s definition to be just one simple line because the type `instruction` contains all information needed to execute a MIPS instruction.

You should now be ready to start coding, testing, and debugging!

## Check for memory leaks with Valgrind

**Note**: Do this only after you pass all `tests.cpp` test cases. You cannot receive points for this unless all `tests.cpp` test cases pass.

In addition to the `tests.cpp` unit tests, the Gradescope autograder has one additional test case that checks for memory leaks and other memory errors. Similar to the formatting test case, this test case is in Gradescope but not `tests.cpp`, so we'll show you the command that the test case runs.

If necessary, review the definition of "memory leak" from [Memory Allocation](https://uncch.instructure.com/users/9947/files/6650292?verifier=Ck555bz4rVkIUYSwscINfbZEJulLQL2N0gV09VUc&wrap=1) (pg. 10, slide title "Heap Rules").

In `instructions.c`, you should call `malloc` twice, and there should be at least one call to `free`. If you did not do this correctly, your program has a memory leak.

To check for memory leaks and other memory errors, we'll use the tool [valgrind](https://valgrind.org/) (run `tldr valgrind` for information). You can simply run `make valgrind`, which is an alias for a longer command defined in our [Makefile](Makefile):

```text
valgrind: $(TESTS)
	valgrind --error-exitcode=1 --leak-check=full --show-leak-kinds=definite --errors-for-leak-kinds=definite ./tests >/dev/null
```

This command runs a memory check on the `tests` executable, which causes heap allocations by calling `create_fields` and `create_instruction`. `tests.cpp` has been written correctly in that whenever either of those functions is called, it calls `free` on the return value when done using it. Similarly, `utils.c` also calls `free` on the return values of those functions, so not everything `malloc`ed in `instructions.c` has to be `free`d in `instructions.c`. However, there may still be a memory leak in your code for either function.

This command generates a lot of output because there are nearly 50 test cases in `tests.cpp`, but you need only look at one line for now. At the bottom, if you see "definitely lost: 0 bytes", we'll consider your code to be free of memory leaks. We'll ignore the fields "indirectly lost" and "possibly lost".

```text
==2770656== LEAK SUMMARY:
==2770656==    definitely lost: 0 bytes in 0 blocks
```

But if you see a non-zero number for "definitely lost", your code has memory leaks:

```text
==2770100== LEAK SUMMARY:
==2770100==    definitely lost: 68 bytes in 17 blocks
```

You actually don't need to read any output just to determine whether or not there's a leak. Because of the options `--error-exitcode=1` and `--errors-for-leak-kinds=definite` in the command, if there is no definite memory leak (and all `tests.cpp` test cases passed), `valgrind` returns exit status 0; otherwise, `valgrind` returns exit status 1. For example,

```text
learncli$ make valgrind >/dev/null 2>&1 # Suppress all output by redirecting stdout to /dev/null and stderr to stdout
learncli$ echo $?  # 0 because the code doesn't have a definite memory leak AND tests executable had exit status 0 (i.e., all test cases passed)
0
learncli$ vim instructions.c  # Edit code to cause a memory leak
learncli$ make valgrind >/dev/null 2>&1
learncli$ echo $?  # nonzero because the code definitely leaks memory
2
```

`echo $?` prints the exit status of the previous command. In general, exit status 0 indicates success, and any other number indicates error (same as the C `main` functions you've written). You will pass the Valgrind test case if the exit status of `make valgrind` is 0; otherwise, you will not.

**Note**: The full `valgrind` command returns either 0 or 1, but `make valgrind` returned 2 above because `make` happens to use exit status 2 to indicate errors (i.e., different conventions). Regardless, this doesn't affect anything because the autograder checks whether `make valgrind`'s exit status is 0, which always indicates success.

### How to debug memory leaks

If you have a memory leak, `valgrind` tells you the function(s) in which it occurred. For example, in example code that leaks memory, the `valgrind` output might include

```text
==2796471== 68 bytes in 17 blocks are definitely lost in loss record 128 of 207
==2796471==    at 0x4846828: malloc (in /usr/libexec/valgrind/vgpreload_memcheck-amd64-linux.so)
==2796471==    by 0x134B0A: create_fields(unsigned int) (instructions.c:26)
==2796471==    by 0x134DD5: create_instruction(unsigned int) (instructions.c:88)
==2796471==    by 0x1357FB: execute_all(unsigned int*, unsigned int, int*, unsigned int*, cli_args) (utils.c:144)
==2796471==    by 0x118060: run_main(int, char**) (main.c:25)
==2796471==    by 0x122C24: MainFunc_AllHexFilesInDataDir_Test::TestBody()::{lambda()#1}::operator()() const (tests.cpp:537)
```

This is a stack trace (similar to stack traces you've seen after running a Java/Python program that throws an exception) that shows where a memory leak occurred. Given this output, the leak technically could be in any of `malloc`, `create_fields`, `create_instruction`, `execute_all`, etc. However, for this lab, if there is an error, it would be from your code in `create_fields` or `create_instruction`. In general, we have already checked our code (e.g., `execute_all` and test cases in `tests.cpp`) for definite leaks, so there shouldn't be any in those places. And, you should trust that the `malloc` source code does not cause memory leaks.

Since we're running the command on the GTest executable, there are a lot of irrelevant GTest function calls, such as `testing::internal::UnitTestImpl::RunAllTests() (gtest.cc:5967)`, that you should ignore. To filter for relevant output, use shell knowledge from Lab 1. For example, you could redirect `valgrind`'s output to a file either by redirecting stderr or by using the `valgrind` flag `--log-file`. Then, you could use a text editor to search for the string "instructions.c" (or other files). Or, to avoid having to open a text editor, you could pipe the output to `grep`.

Lastly, Valgrind tells us the line number(s) at which memory was allocated but not freed. For example, in the above output for our example code, `instructions.c:88` was indeed a line that caused a memory leak. Given Valgrind's detailed output, memory leaks should not be hard to debug.

## Ignored nuances

There are some nuances in MIPS architecture that are ignored in this lab.

1. `$0` (the "zero register") is read-only and always contains the value 0. However, for this lab, all 32 registers are considered general-purpose registers.
2. Do not consider the sign extension of immediate values. In the functions `sll`, `sra`, `add`, `sub`, etc., all updates to `registers` should be straightforward (i.e., one line each to perform the register update).
3. Our simulator stops executing instructions only when the last instruction (the one at the bottom of the file) is executed. However, this approach is unrealistic because it assumes that if the program contains instructions numbered 1, 2, 3, ..., n, they are executed in the same order 1, 2, 3, ..., n. If there are control flow instructions, the order of execution probably isn't 1, 2, 3, ..., n, so the program also needs to stop executing when a special "halt" instruction is executed. In MIPS, this would be the execution of a `syscall` instruction when register `$2` contains the value 10. But our simulator's approach is okay in this context because it does not support control flow instructions.

## Submit your assignment

See the [instructions for assignment submission](https://github.com/Comp211-FA24/lab-00?tab=readme-ov-file#submit-your-assignment).
