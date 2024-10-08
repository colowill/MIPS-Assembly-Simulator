#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

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

// @note For info about union, see README.md and/or
// https://www.tutorialspoint.com/cprogramming/c_unions.htm
typedef union {
    r_fields r;
    i_fields i;
} fields;

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

#endif  // TYPES_H
