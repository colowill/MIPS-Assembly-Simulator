#include "instructions.h"

#include <stdio.h>
#include <stdlib.h>

#include "constants.h"
#include "types.h"

// This is given to you, don't edit
unsigned int bit_select(unsigned int num, unsigned int start_bit,
                        unsigned int end_bit) {
    start_bit++;
    num <<= (UNSIGNED_INT_NUM_BITS - start_bit);
    num >>= (UNSIGNED_INT_NUM_BITS - start_bit + end_bit);
    return num;
}

instruction_type determine_instruction_type(uint32_t instruct) {
    // TODO:
    return I_TYPE;
}

fields* create_fields(uint32_t instruct) {
    fields* rv = (fields*)malloc(sizeof(fields));
    // TODO:
    return rv;
}

instruction_name determine_instruction_name(uint32_t instruct) {
    // TODO:
    return SLL;
}

instruction* create_instruction(uint32_t instruct) {
    instruction* rv = (instruction*)malloc(sizeof(instruction));
    // TODO:
    return rv;
}

// This is given to you, don't edit
void execute_instruction(instruction* instruct, int32_t* registers,
                         uint32_t* pc) {
    instruct->execute(instruct->_fields, registers, pc);
}

// For info about all functions below this comment, see comments at the bottom
// of instructions.h

void sll(fields fields, int32_t* registers, uint32_t* pc) {
    r_fields r_fields = fields.r;
    // TODO:
}

void sra(fields fields, int32_t* registers, uint32_t* pc) {
    r_fields r_fields = fields.r;
    // TODO:
}

void add(fields fields, int32_t* registers, uint32_t* pc) {
    r_fields r_fields = fields.r;
    // TODO:
}

void sub(fields fields, int32_t* registers, uint32_t* pc) {
    r_fields r_fields = fields.r;
    // TODO:
}

// Can't name this "and" because "and" is a reserved keyword
void and_op(fields fields, int32_t* registers, uint32_t* pc) {
    r_fields r_fields = fields.r;
    // TODO:
}

// Can't name this "or" because "or" is a reserved keyword
void or_op(fields fields, int32_t* registers, uint32_t* pc) {
    r_fields r_fields = fields.r;
    // TODO:
}

void nor(fields fields, int32_t* registers, uint32_t* pc) {
    r_fields r_fields = fields.r;
    // TODO:
}

void addi(fields fields, int32_t* registers, uint32_t* pc) {
    i_fields i_fields = fields.i;
    // TODO:
}

void andi(fields fields, int32_t* registers, uint32_t* pc) {
    i_fields i_fields = fields.i;
    // TODO:
}

void ori(fields fields, int32_t* registers, uint32_t* pc) {
    i_fields i_fields = fields.i;
    // TODO:
}
