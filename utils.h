#ifndef utils_H
#define utils_H

#include <unistd.h>

#include "constants.h"
#include "instructions.h"
#include "types.h"

typedef struct {
    char* filepath;
    bool disp_array;
    bool step_mode;
    bool disp_hex;
} cli_args;

/**
 * Parses command line arguments and return a cli_args struct
 *
 * Exits on failure due to invalid user input
 *
 * @param argc
 * @param argv
 * @return cli_args
 */
cli_args parse_cli(int argc, char* argv[]);

/**
 * Pretty prints registers and PC in table format
 *
 * @param registers
 * @param pc
 * @param disp_array true to print as array for autograding, else false to print
 * table
 * @param disp_hex true to print values in hex, else false to print in base-10
 * @return void
 */
void print_state(int32_t* registers, uint32_t pc, bool disp_array,
                 bool disp_hex);

/**
 * Checks whether pc is valid (i.e., is a multiple of WORD_SIZE)
 *
 * @param pc
 * @return true if pc is valid, else false
 */
bool validate_pc(uint32_t pc);

/**
 * Executes instructions until the final instruction is executed (assuming no
 * jump/branch instructions), mutating registers and pc
 *
 * If pc is invalid (i.e., not a multiple of WORD_SIZE) during execution, prints
 * error message and exits
 *
 * @note Executing until the final instruction is simple and sufficient for this
 * lab because there are no jump/branch instructions. However, this is
 * unrealistic. A MIPS assembly program should actually exit when a syscall is
 * executed when $v0 contains the value 10. Or, for simplicity, the simulator
 * could exit when a special hardcoded instruction is executed, e.g., 0xdeadbeef
 * @param instructions pointer to instructions
 * @param num_instructions number of instructions
 * @param registers
 * @param pc
 * @param flags cli flags
 * @return void (but mutates registers and pc)
 */
void execute_all(uint32_t* instructions, uint32_t num_instructions,
                 int32_t* registers, uint32_t* pc, cli_args flags);

/**
 * Parses a file with MIPS instructions in hex format and populates a uint32_t
 * array with its instructions
 *
 * To generate a file with MIPS instructions in hex format, see the instructions
 * given by ./main -m (or check comments in main.c)
 *
 * If filepath is invalid, exits with error message
 *
 * @param filepath
 * @param instructions instructions array to be filled
 * @return number of instructions read on success
 */
uint32_t hex_instruction_file_to_array(const char* filepath,
                                       uint32_t* instructions);

#endif  // utils_H
