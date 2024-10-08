#include <cstring>
#include <filesystem>
#include <unordered_map>
#include <vector>

#include "constants.h"
#include "gtest/gtest.h"
#include "instructions.h"
#include "main.c"

void run_with_signal_catching(void (*test_body)());

#define SAFE_TEST(TestSuiteName, TestName, TestBody) \
    TEST(TestSuiteName, TestName) { run_with_signal_catching([]() TestBody); }

namespace fs = std::filesystem;

const fs::path DATA_DIR("data");

// These variable names represent MIPS instructions
// E.g., ADD_0_0_0 means add $0, $0, $0
// ADDI_8_0_0x000A means addi $0, $0, 0x000A
// All instruction values are given in 32-bit binary format

const uint32_t ADD_0_0_0 = 0b00000000000000000000000000100000;
const uint32_t SUB_0_0_0 = 0b00000000000000000000000000100010;
const uint32_t AND_0_0_0 = 0b00000000000000000000000000100100;
const uint32_t OR_0_0_0 = 0b00000000000000000000000000100101;
const uint32_t NOR_0_0_0 = 0b00000000000000000000000000100111;

const uint32_t ADDI_8_0_0x000A = 0b00100000000010000000000000001010;
const uint32_t ANDI_10_9_1 = 0b00110001001010100000000000000001;
const uint32_t ORI_10_9_1 = 0b00110101001010100000000000000001;

const uint32_t ADD_17_9_25 = 0b00000001001110011000100000100000;
const uint32_t AND_12_9_27 = 0b00000001001110110110000000100100;
const uint32_t ADDI_17_9_0x04D2 = 0b00100001001100010000010011010010;
const uint32_t ANDI_17_9_12 = 0b00110001001100010000000000001100;

const uint32_t SLL_8_9_5 = 0b00000000000010010100000101000000;
const uint32_t SRA_8_9_5 = 0b00000000000010010100000101000011;
const uint32_t ADD_3_1_2 = 0b00000000001000100001100000100000;
const uint32_t SUB_3_1_2 = 0b00000000001000100001100000100010;
const uint32_t AND_3_1_3 = 0b00000000001000110001100000100100;
const uint32_t OR_3_1_3 = 0b00000000001000110001100000100101;
const uint32_t NOR_17_9_10 = 0b00000001001010101000100000100111;

const uint32_t ADDI_17_9_12 = 0b00100001001100010000000000001100;
const uint32_t ORI_17_9_12 = 0b00110101001100010000000000001100;

const uint32_t SLL_11_9_3 = 0b00000000000010010101100011000000;
const uint32_t SRA_11_9_3 = 0b00000000000010010101100011000011;
const uint32_t ADDI_11_9_3 = 0b00100001001010110000000000000011;

// Map input file for main executable to expected output
// Expected output has 33 values
// First 32 values are the 32 general-purpose register values
// Last value is PC
const std::unordered_map<std::string, std::string> MAIN_FUNC_EXPECTED_OUTPUT = {
    {"data/addi_add_sub_ori.hex",
     "[21834, 43668, 21834, 21835, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "
     "0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 16]\n"},
    {"data/addi_negative.hex",
     "[0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "
     "0, 0, 0, 0, 0, 0, 0, 0, 0, 4]\n"},
    {"data/addi.hex",
     "[0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "
     "0, 0, 0, 0, 0, 0, 0, 0, 8]\n"},
    {"data/ori_andi.hex",
     "[0, 0, 0, 0, 0, 0, 0, 0, 5, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "
     "0, 0, 0, 0, 0, 0, 0, 0, 0, 8]\n"},
    {"data/ori_ori_nor.hex",
     "[0, 0, 0, 0, 0, 0, 0, 0, 4, 10, -15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "
     "0, 0, 0, 0, 0, 0, 0, 0, 0, 12]\n"},
    {"data/ori_sll.hex",
     "[0, 0, 0, 0, 0, 0, 0, 0, 62, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "
     "0, 0, 0, 0, 0, 0, 0, 0, 0, 8]\n"},
    {"data/ori_sra.hex",
     "[0, 0, 0, 0, 0, 0, 0, 0, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "
     "0, 0, 0, 0, 0, 0, 0, 0, 0, 8]\n"},
    {"data/ori.hex",
     "[0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "
     "0, 0, 0, 0, 0, 0, 0, 0, 0, 4]\n"}};

SAFE_TEST(DetermineInstructionType, RType, {
    EXPECT_EQ(R_TYPE, determine_instruction_type(ADD_0_0_0));
    EXPECT_EQ(R_TYPE, determine_instruction_type(SUB_0_0_0));
    EXPECT_EQ(R_TYPE, determine_instruction_type(AND_0_0_0));
    EXPECT_EQ(R_TYPE, determine_instruction_type(OR_0_0_0));
    EXPECT_EQ(R_TYPE, determine_instruction_type(NOR_0_0_0));
    EXPECT_EQ(R_TYPE, determine_instruction_type(SLL_11_9_3));
    EXPECT_EQ(R_TYPE, determine_instruction_type(SRA_11_9_3));
})

SAFE_TEST(DetermineInstructionType, IType, {
    EXPECT_EQ(I_TYPE, determine_instruction_type(ADDI_8_0_0x000A));
    EXPECT_EQ(I_TYPE, determine_instruction_type(ANDI_10_9_1));
    EXPECT_EQ(I_TYPE, determine_instruction_type(ORI_10_9_1));
})

SAFE_TEST(CreateFields, sll, {
    fields* r_fields = create_fields(SLL_8_9_5);

    EXPECT_EQ(0b00000, r_fields->r.rs);
    EXPECT_EQ(0b01001, r_fields->r.rt);
    EXPECT_EQ(0b01000, r_fields->r.rd);
    EXPECT_EQ(0b00101, r_fields->r.shamt);

    free(r_fields);
})

SAFE_TEST(CreateFields, sra, {
    fields* r_fields = create_fields(SRA_11_9_3);

    EXPECT_EQ(0b00000, r_fields->r.rs);
    EXPECT_EQ(0b01001, r_fields->r.rt);
    EXPECT_EQ(0b01011, r_fields->r.rd);
    EXPECT_EQ(0b00011, r_fields->r.shamt);

    free(r_fields);
})

SAFE_TEST(CreateFields, add, {
    fields* r_fields = create_fields(ADD_17_9_25);

    EXPECT_EQ(0b01001, r_fields->r.rs);
    EXPECT_EQ(0b11001, r_fields->r.rt);
    EXPECT_EQ(0b10001, r_fields->r.rd);
    EXPECT_EQ(0, r_fields->r.shamt);

    free(r_fields);
})

SAFE_TEST(CreateFields, sub, {
    fields* r_fields = create_fields(SUB_3_1_2);

    EXPECT_EQ(0b00001, r_fields->r.rs);
    EXPECT_EQ(0b00010, r_fields->r.rt);
    EXPECT_EQ(0b00011, r_fields->r.rd);
    EXPECT_EQ(0, r_fields->r.shamt);

    free(r_fields);
})

SAFE_TEST(CreateFields, and, {
    fields* r_fields = create_fields(AND_12_9_27);

    EXPECT_EQ(0b01001, r_fields->r.rs);
    EXPECT_EQ(0b11011, r_fields->r.rt);
    EXPECT_EQ(0b01100, r_fields->r.rd);
    EXPECT_EQ(0, r_fields->r.shamt);

    free(r_fields);
})

SAFE_TEST(CreateFields, or, {
    fields* r_fields = create_fields(OR_3_1_3);

    EXPECT_EQ(0b00001, r_fields->r.rs);
    EXPECT_EQ(0b00011, r_fields->r.rt);
    EXPECT_EQ(0b00011, r_fields->r.rd);
    EXPECT_EQ(0, r_fields->r.shamt);

    free(r_fields);
})

SAFE_TEST(CreateFields, nor, {
    fields* r_fields = create_fields(NOR_17_9_10);

    EXPECT_EQ(0b01001, r_fields->r.rs);
    EXPECT_EQ(0b01010, r_fields->r.rt);
    EXPECT_EQ(0b10001, r_fields->r.rd);
    EXPECT_EQ(0, r_fields->r.shamt);

    free(r_fields);
})

SAFE_TEST(CreateFields, addi, {
    fields* i_fields = create_fields(ADDI_17_9_0x04D2);

    EXPECT_EQ(0b01001, i_fields->i.rs);
    EXPECT_EQ(0b10001, i_fields->i.rt);
    EXPECT_EQ(0b0000010011010010, i_fields->i.immediate);

    free(i_fields);
})

SAFE_TEST(CreateFields, andi, {
    fields* i_fields = create_fields(ANDI_17_9_12);

    EXPECT_EQ(0b01001, i_fields->i.rs);
    EXPECT_EQ(0b10001, i_fields->i.rt);
    EXPECT_EQ(0b0000000000001100, i_fields->i.immediate);

    free(i_fields);
})

SAFE_TEST(CreateFields, ori, {
    fields* i_fields = create_fields(ORI_10_9_1);

    EXPECT_EQ(0b01001, i_fields->i.rs);
    EXPECT_EQ(0b01010, i_fields->i.rt);
    EXPECT_EQ(0b0000000000000001, i_fields->i.immediate);

    free(i_fields);
})

SAFE_TEST(DetermineInstructionName, sll,
          { EXPECT_EQ(SLL, determine_instruction_name(SLL_8_9_5)); })

SAFE_TEST(DetermineInstructionName, sra,
          { EXPECT_EQ(SRA, determine_instruction_name(SRA_8_9_5)); })

SAFE_TEST(DetermineInstructionName, add,
          { EXPECT_EQ(ADD, determine_instruction_name(ADD_3_1_2)); })

SAFE_TEST(DetermineInstructionName, sub,
          { EXPECT_EQ(SUB, determine_instruction_name(SUB_3_1_2)); })

SAFE_TEST(DetermineInstructionName, and,
          { EXPECT_EQ(AND, determine_instruction_name(AND_3_1_3)); })

SAFE_TEST(DetermineInstructionName, or,
          { EXPECT_EQ(OR, determine_instruction_name(OR_3_1_3)); })

SAFE_TEST(DetermineInstructionName, nor,
          { EXPECT_EQ(NOR, determine_instruction_name(NOR_17_9_10)); })

SAFE_TEST(DetermineInstructionName, addi,
          { EXPECT_EQ(ADDI, determine_instruction_name(ADDI_17_9_12)); })

SAFE_TEST(DetermineInstructionName, andi,
          { EXPECT_EQ(ANDI, determine_instruction_name(ANDI_17_9_12)); })

SAFE_TEST(DetermineInstructionName, ori,
          { EXPECT_EQ(ORI, determine_instruction_name(ORI_17_9_12)); })

SAFE_TEST(CreateInstruction, sll, {
    instruction* instr = create_instruction(SLL_11_9_3);

    EXPECT_EQ(0b01011, instr->_fields.r.rd);
    EXPECT_EQ(0b01001, instr->_fields.r.rt);
    EXPECT_EQ(0b00011, instr->_fields.r.shamt);
    EXPECT_EQ(sll, instr->execute);

    free(instr);
})

SAFE_TEST(CreateInstruction, sra, {
    instruction* instr = create_instruction(SRA_11_9_3);

    EXPECT_EQ(0b01011, instr->_fields.r.rd);
    EXPECT_EQ(0b01001, instr->_fields.r.rt);
    EXPECT_EQ(0b00011, instr->_fields.r.shamt);
    EXPECT_EQ(sra, instr->execute);

    free(instr);
})

SAFE_TEST(CreateInstruction, add, {
    instruction* instr = create_instruction(ADD_3_1_2);

    EXPECT_EQ(0b00011, instr->_fields.r.rd);
    EXPECT_EQ(0b00001, instr->_fields.r.rs);
    EXPECT_EQ(0b00010, instr->_fields.r.rt);
    EXPECT_EQ(add, instr->execute);

    free(instr);
})

SAFE_TEST(CreateInstruction, sub, {
    instruction* instr = create_instruction(SUB_3_1_2);

    EXPECT_EQ(0b00011, instr->_fields.r.rd);
    EXPECT_EQ(0b00001, instr->_fields.r.rs);
    EXPECT_EQ(0b00010, instr->_fields.r.rt);
    EXPECT_EQ(sub, instr->execute);

    free(instr);
})

SAFE_TEST(CreateInstruction, and, {
    instruction* instr = create_instruction(AND_12_9_27);

    EXPECT_EQ(0b01100, instr->_fields.r.rd);
    EXPECT_EQ(0b01001, instr->_fields.r.rs);
    EXPECT_EQ(0b11011, instr->_fields.r.rt);
    EXPECT_EQ(and_op, instr->execute);

    free(instr);
})

SAFE_TEST(CreateInstruction, or, {
    instruction* instr = create_instruction(OR_3_1_3);

    EXPECT_EQ(0b00011, instr->_fields.r.rd);
    EXPECT_EQ(0b00001, instr->_fields.r.rs);
    EXPECT_EQ(0b00011, instr->_fields.r.rt);
    EXPECT_EQ(or_op, instr->execute);

    free(instr);
})

SAFE_TEST(CreateInstruction, nor, {
    instruction* instr = create_instruction(NOR_17_9_10);

    EXPECT_EQ(0b10001, instr->_fields.r.rd);
    EXPECT_EQ(0b01001, instr->_fields.r.rs);
    EXPECT_EQ(0b01010, instr->_fields.r.rt);
    EXPECT_EQ(nor, instr->execute);

    free(instr);
})

SAFE_TEST(CreateInstruction, addi, {
    instruction* instr = create_instruction(ADDI_11_9_3);

    EXPECT_EQ(0b01011, instr->_fields.i.rt);
    EXPECT_EQ(0b01001, instr->_fields.i.rs);
    EXPECT_EQ(0b0000000000000011, instr->_fields.i.immediate);
    EXPECT_EQ(addi, instr->execute);

    free(instr);
})

SAFE_TEST(CreateInstruction, andi, {
    instruction* instr = create_instruction(ANDI_17_9_12);

    EXPECT_EQ(0b10001, instr->_fields.i.rt);
    EXPECT_EQ(0b01001, instr->_fields.i.rs);
    EXPECT_EQ(0b0000000000001100, instr->_fields.i.immediate);
    EXPECT_EQ(andi, instr->execute);

    free(instr);
})

SAFE_TEST(CreateInstruction, ori, {
    instruction* instr = create_instruction(ORI_10_9_1);

    EXPECT_EQ(0b01010, instr->_fields.i.rt);
    EXPECT_EQ(0b01001, instr->_fields.i.rs);
    EXPECT_EQ(0b0000000000000001, instr->_fields.i.immediate);
    EXPECT_EQ(ori, instr->execute);

    free(instr);
})

SAFE_TEST(sll, Basic, {
    uint32_t pc = 0;
    int32_t registers[NUM_REGISTERS] = {0};
    registers[2] = 4;
    fields f;
    f.r.rt = 2;
    f.r.rd = 3;
    f.r.shamt = 2;

    sll(f, registers, &pc);

    EXPECT_EQ(16, registers[3]);
    EXPECT_EQ(4, pc);
})

SAFE_TEST(sra, Basic, {
    uint32_t pc = 4;
    int32_t registers[NUM_REGISTERS] = {0};
    registers[3] = -64;
    fields f;
    f.r.rt = 3;
    f.r.rd = 4;
    f.r.shamt = 2;

    sra(f, registers, &pc);

    EXPECT_EQ(-16, registers[4]);
    EXPECT_EQ(8, pc);
})

SAFE_TEST(add, Basic, {
    uint32_t pc = 8;
    int32_t registers[NUM_REGISTERS] = {0};
    registers[4] = 3;
    registers[5] = 5;
    fields f;
    f.r.rs = 4;
    f.r.rt = 5;
    f.r.rd = 6;

    add(f, registers, &pc);

    EXPECT_EQ(8, registers[6]);
    EXPECT_EQ(12, pc);
})

SAFE_TEST(sub, Basic, {
    uint32_t pc = 12;
    int32_t registers[NUM_REGISTERS] = {0};
    registers[7] = 12;
    registers[8] = 5;
    fields f;
    f.r.rs = 7;
    f.r.rt = 8;
    f.r.rd = 9;

    sub(f, registers, &pc);

    EXPECT_EQ(7, registers[9]);
    EXPECT_EQ(16, pc);
})

SAFE_TEST(and, Basic, {
    uint32_t pc = 16;
    int32_t registers[NUM_REGISTERS] = {0};
    registers[12] = 0xf0f0;
    registers[11] = 0xff00;
    fields f;
    f.r.rs = 12;
    f.r.rt = 11;
    f.r.rd = 10;

    and_op(f, registers, &pc);

    EXPECT_EQ(0xf000, registers[10]);
    EXPECT_EQ(20, pc);
})

SAFE_TEST(or, Basic, {
    uint32_t pc = 20;
    int32_t registers[NUM_REGISTERS] = {0};
    registers[17] = 0xf0f0;
    registers[9] = 0xff00;
    fields f;
    f.r.rs = 17;
    f.r.rt = 9;
    f.r.rd = 31;

    or_op(f, registers, &pc);

    EXPECT_EQ(0xfff0, registers[31]);
    EXPECT_EQ(24, pc);
})

SAFE_TEST(nor, Basic, {
    uint32_t pc = 24;
    int32_t registers[NUM_REGISTERS] = {0};
    registers[9] = 0xf0f0;
    registers[6] = 0xff00;
    fields f;
    f.r.rs = 9;
    f.r.rt = 6;
    f.r.rd = 3;

    nor(f, registers, &pc);

    // Note that 0xf0f0 and 0xff00 are really 0x0000f0f0 and 0x0000ff00
    // So when the nor is done, the first 4 hexits are ffff
    EXPECT_EQ(0xffff000f, registers[3]);
    EXPECT_EQ(28, pc);
})

SAFE_TEST(addi, Basic, {
    uint32_t pc = 28;
    int32_t registers[NUM_REGISTERS] = {0};
    registers[5] = 3;
    fields f;
    f.i.rs = 5;
    f.i.rt = 15;
    f.i.immediate = 17;

    addi(f, registers, &pc);

    EXPECT_EQ(20, registers[15]);
    EXPECT_EQ(32, pc);
})

SAFE_TEST(andi, Basic, {
    uint32_t pc = 32;
    int32_t registers[NUM_REGISTERS] = {0};
    registers[12] = 0xf0f0;
    fields f;
    f.i.rs = 12;
    f.i.rt = 16;
    f.i.immediate = 0xff00;

    andi(f, registers, &pc);

    EXPECT_EQ(0xf000, registers[16]);
    EXPECT_EQ(36, pc);
})

SAFE_TEST(ori, Basic, {
    uint32_t pc = 36;
    int32_t registers[NUM_REGISTERS] = {0};
    registers[30] = 0xf0f0;
    fields f;
    f.i.rs = 30;
    f.i.rt = 29;
    // Not using 0xff00 because it exceeds the range of int16_t
    f.i.immediate = 0x7f00;

    ori(f, registers, &pc);

    EXPECT_EQ(0xfff0, registers[29]);
    EXPECT_EQ(40, pc);
})

// This test runs main.c's run_main function on data/*.hex
// and compares the output with our expected output
// To see how to use the main executable, see README section Input/output
// And/or see a help message by running ./main -h (assuming you compiled with
// make)
TEST(MainFunc, AllHexFilesInDataDir) {
    run_with_signal_catching([]() {
        const int argc = 3;
        for (const fs::directory_entry& dir_entry :
             fs::recursive_directory_iterator(DATA_DIR)) {
            if (dir_entry.is_regular_file() &&
                dir_entry.path().extension() == ".hex") {
                std::string path_str = dir_entry.path().string();
                // Pass flag -a to print registers as array (not table)
                std::vector<std::string> args = {"./main", "-a", path_str};
                // Need to dynamically allocate argv
                // new is similar to malloc
                char** argv = new char*[argc + 1];
                for (int i = 0; i < argc; i++) {
                    argv[i] = new char[args[i].size() + 1];
                    std::strcpy(argv[i], args[i].c_str());
                }
                argv[argc] = NULL;

                // Since run_main prints to stdout, capture stdout
                int pipe_fd[2];
                pipe(pipe_fd);
                int original_stdout_fd = dup(STDOUT_FILENO);
                dup2(pipe_fd[1], STDOUT_FILENO);
                close(pipe_fd[1]);

                run_main(argc, argv);

                fflush(stdout);
                dup2(original_stdout_fd, STDOUT_FILENO);
                close(original_stdout_fd);

                char buffer[1024];
                ssize_t bytes_read =
                    read(pipe_fd[0], buffer, sizeof(buffer) - 1);
                buffer[bytes_read] = '\0';
                close(pipe_fd[0]);

                std::string captured_stdout = buffer;
                EXPECT_STREQ(MAIN_FUNC_EXPECTED_OUTPUT.at(path_str).c_str(),
                             captured_stdout.c_str());

                // delete is similar to free
                for (int i = 0; i < argc; i++) delete[] argv[i];
                delete[] argv;
            }
        }
    });
}

void run_with_signal_catching(void (*test_body)()) {
    int pipefd[2];

    if (pipe(pipefd) != 0) {
        FAIL() << "run_with_signal_handling: pipe call failed";
        return;
    }

    pid_t pid = fork();
    if (pid == 0) {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        dup2(pipefd[1], STDERR_FILENO);
        close(pipefd[1]);
        test_body();
        if (::testing::Test::HasFailure()) _Exit(1);
        _Exit(0);
    } else {
        close(pipefd[1]);
        std::ostringstream output;
        char buffer[1024];
        ssize_t count;
        while ((count = read(pipefd[0], buffer, sizeof(buffer))) > 0)
            output.write(buffer, count);
        close(pipefd[0]);
        std::string raw_output = output.str();
        std::string trimmed_output =
            raw_output.substr(0, raw_output.find_last_not_of("\n") + 1);
        int status;
        waitpid(pid, &status, 0);
        if (WIFSIGNALED(status)) {
            int signal = WTERMSIG(status);
            std::string error_message = trimmed_output;
            if (!trimmed_output.empty()) error_message.append("\n");
            error_message.append("Test crashed with signal: " +
                                 std::string(strsignal(signal)));
            FAIL() << error_message;
        } else if (WIFEXITED(status)) {
            if (WEXITSTATUS(status) != 0) FAIL() << trimmed_output;
        }
    }
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
