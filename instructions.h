#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include <stdbool.h>
#include <stdint.h>

#include "types.h"

/**
 * Selects a range of bits from num
 *
 * @note This is from Lab 2, and you are given a correct implementation
 * @param num
 * @param start_bit interpreted as most significant bit in the selected range
 * @param end_bit interpreted as least significant bit in the selected range
 * @return number formed by bits of num in the range start_bit to end_bit,
 * inclusive of both, 0-indexed (i.e., the least significant bit is the 0th bit)
 */
unsigned int bit_select(unsigned int num, unsigned int start_bit,
                        unsigned int end_bit);

/**
 * Given MIPS instruction in 32-bit form, determines whether it is R-type or
 * I-type and returns that type
 *
 * @note Although J-type MIPS instructions also exist, you need only distinguish
 * between R-type and I-type for this lab
 * @note See [MIPS Cheat
 * Sheet](https://uncch.instructure.com/users/9947/files/4534610?verifier=0lJburrtzSqIT791v3YyAlhY3ZBq0MykyBPR1nY6&wrap=1)
 * @param instruction
 * @return R_TYPE | I_TYPE
 */
instruction_type determine_instruction_type(uint32_t instruct);

/**
 * Given a MIPS instruction in 32-bit form, returns pointer to initialized
 * fields based on the instruction's bit values
 *
 * @note fields is a union type that will be either r_fields struct or i_fields
 * struct
 * @note To complete this, reference the top left of the [MIPS Cheat
 * Sheet](https://uncch.instructure.com/users/9947/files/4534610?verifier=0lJburrtzSqIT791v3YyAlhY3ZBq0MykyBPR1nY6&wrap=1)
 * @note Use the constants given in constants.h. Once the code identifies
 * whether the instruction is R-type or I-type, you need only extract the bits
 * at the correct positions (using bit_select) to initialize r_fields or
 * i_fields. For example, for an I-type instruction, bits 15-0 are the immediate
 * field, so the constants IMMEDIATE_START_BIT and IMMEDIATE_END_BIT are defined
 * as 15 and 0, respectively
 * @param instruction
 * @return fields*
 */
fields* create_fields(uint32_t instruct);

/**
 * Given a MIPS instruction in 32-bit form, returns the name of the instruction
 *
 * @note This function should use determine_instruction_type and some additional
 * logic involving bit_select
 * @note This function should be used to determine which function should be
 * assigned to instruction.execute, see README.md section Function
 * pointer
 * @param instruction
 * @return SLL | SRA | ADD | SUB | AND | OR | NOR | ADDI | ANDI | ORI
 */
instruction_name determine_instruction_name(uint32_t instruct);

/**
 * Initializes instruction struct given 32-bit representation of MIPS
 * instruction
 *
 * Specifically, initializes the struct members _fields and execute
 *
 * @note _fields is initialized using create_fields
 * @note execute is a function pointer that you can set like so: rv->execute =
 * sll (sll is the name of the sll function near the bottom of this file, and rv
 * is the result of malloc(sizeof(instruction)))
 * @param instruct
 * @return instruction*
 */
instruction* create_instruction(uint32_t instruct);

/**
 * Executes the given instruction, mutating pc and probably registers
 *
 * @param instruct
 * @param registers possibly mutated by instruction execution
 * @param pc mutated by instruction execution
 */
void execute_instruction(instruction* instruct, int32_t* registers,
                         uint32_t* pc);

/**
 * When creating an instruction struct, one of the below functions is assigned
 * to the struct member named "execute". See README.md section Function pointer
 * for more details
 *
 * Each function must mutate pc and registers (these are pointers)
 * (r_|i_)fields should not be mutated (not a pointer)
 *
 * registers is an int32_t array of size 32 that represents the 32 MIPS
 * registers
 * registers[0] is $0, registers[1] is $1, etc. See MIPS cheat
 * sheet pg. 3
 *
 * pc is a pointer to the program counter, which stores the address of the
 * instruction to be executed
 * Before the function is called, pc is the address of the instruction to be
 * executed
 * When the function returns, pc should be the address of the next
 * instruction to be executed, which is *pc + WORD_SIZE for all instructions in
 * this lab because there aren't control flow instructions
 * But if there were control flow instructions, this would not be the case
 */

void sll(fields fields, int32_t* registers, uint32_t* pc);
void sra(fields fields, int32_t* registers, uint32_t* pc);
void add(fields fields, int32_t* registers, uint32_t* pc);
void sub(fields fields, int32_t* registers, uint32_t* pc);
// Can't name this "and" because "and" is a reserved keyword
void and_op(fields fields, int32_t* registers, uint32_t* pc);
// Can't name this "or" because "or" is a reserved keyword
void or_op(fields fields, int32_t* registers, uint32_t* pc);
void nor(fields fields, int32_t* registers, uint32_t* pc);

void addi(fields fields, int32_t* registers, uint32_t* pc);
void andi(fields fields, int32_t* registers, uint32_t* pc);
void ori(fields fields, int32_t* registers, uint32_t* pc);

#endif  // INSTRUCTIONS_H
