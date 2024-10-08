/**
 * These constants are needed to parse a MIPS instruction's fields when
 * it is represented as a 32-bit number
 *
 * Specifically, given a MIPS instruction as a uint32_t, pass START_BIT and
 * END_BIT values to bit_select to select a range of bits (i.e., the range of
 * bits representing opcode, rs, rt, etc.)
 *
 * #define is used instead of const to prevent multiple definition errors and to
 * be able to use these in switch statements
 */

#ifndef CONSTANTS_H
#define CONSTANTS_H

#define OPCODE_START_BIT 31
#define OPCODE_END_BIT 26

// R-type and I-type instructions have rs and rt
#define RS_START_BIT 25
#define RS_END_BIT 21
#define RT_START_BIT 20
#define RT_END_BIT 16

// R-type instruction has rd, shamt, and funct
#define RD_START_BIT 15
#define RD_END_BIT 11
#define SHAMT_START_BIT 10
#define SHAMT_END_BIT 6
#define FUNCT_START_BIT 5
#define FUNCT_END_BIT 0

// I-type instruction has immediate
#define IMMEDIATE_START_BIT 15
#define IMMEDIATE_END_BIT 0

#define R_TYPE_OPCODE 0b000000
#define ADDI_OPCODE 0b001000
#define ANDI_OPCODE 0b001100
#define ORI_OPCODE 0b001101

#define SLL_FUNCT 0b000000
#define SRA_FUNCT 0b000011
#define ADD_FUNCT 0b100000
#define SUB_FUNCT 0b100010
#define AND_FUNCT 0b100100
#define OR_FUNCT 0b100101
#define NOR_FUNCT 0b100111

// For bit_select
#define UNSIGNED_INT_NUM_BITS sizeof(unsigned int) * 8

#define INITIAL_PC 0
#define NUM_REGISTERS 32
// Each instruction is 4 bytes
#define WORD_SIZE 4
// Definitely lower than the actual max number, but this is a suitable limit
#define MAX_INSTRUCTIONS 1000

#endif  // CONSTANTS_H
