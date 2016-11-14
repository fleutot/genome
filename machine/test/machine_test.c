/*----------------------------------------------------------------------------
Copyright (c) 2013 Gauthier Fleutot Östervall
----------------------------------------------------------------------------*/
// Module under test.
#include "../machine.h"
#include "../machine.c" // Including c file in order to test internal
                        // functionality. 

#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>

//******************************************************************************
// Module macros
//******************************************************************************
#define TEST_START_PRINT()    do {              \
        printf("Running %s...", __func__);      \
        fflush(stdout);                         \
    } while (0)

#define TEST_END_PRINT()  do {                  \
        printf("OK.\n");                        \
    } while (0)

//******************************************************************************
// Module constants
//******************************************************************************

//******************************************************************************
// Module variables
//******************************************************************************

//******************************************************************************
// Function prototypes
//******************************************************************************
static void test_machine_init(void);
static void test_machine_command_create(void);
static void test_machine_command_with_clamp(void);
static void test_machine_command_valid_check(void);

//******************************************************************************
// Function definitions
//******************************************************************************
int main(void)
{
    test_machine_init();
    test_machine_command_create();
    test_machine_command_with_clamp();
    test_machine_command_valid_check();
    printf("All tests passed.\n");
}


//******************************************************************************
// Internal functions
//******************************************************************************
static void test_machine_init(void)
{
    TEST_START_PRINT();
    register_value_t data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
                               15,16, 17};

    bool init_result;

    init_result = machine_init(data, NB_REGISTERS + 1);
    assert(init_result == false);

    init_result = machine_init(data, NB_REGISTERS);
    assert(init_result == true);

    assert(machine_result_get() == data[0]);
    TEST_END_PRINT();
}

static void test_machine_command_create(void)
{
    TEST_START_PRINT();
    register_value_t data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
                               15,16, 17};

    command_t *command = machine_command_create(reg_A, ADD, reg_B, reg_C);

    machine_init(data, NB_REGISTERS);
    machine_command_run(command);
    register_value_t result = machine_result_get();

    assert(result == data[1] + data[2]);

    machine_command_destroy(command);
    TEST_END_PRINT();
}


static void test_machine_command_with_clamp(void)
{
    TEST_START_PRINT();
    register_value_t data[] = {
        0,                          // A
        REGISTER_MAX - 10, 20,      // B, C
        REGISTER_MIN + 10, -20,     // D, E
        REGISTER_MAX / 2 + 1, 2,    // F, G
        REGISTER_MIN / 2 - 1, 2,    // H, I
        42, 0,                      // J, K
        11, 12, 13, 14, 15, 16, 17
    };

    machine_init(data, NB_REGISTERS);

    command_t *command;
    register_value_t result;

    // Clamping of ADD.
    command = machine_command_create(reg_A, ADD, reg_B, reg_C);
    machine_command_run(command);
    result = machine_result_get();
    assert(result == REGISTER_MAX);
    machine_command_destroy(command);

    command = machine_command_create(reg_A, ADD, reg_D, reg_E);
    machine_command_run(command);
    result = machine_result_get();
    assert(result == REGISTER_MIN);
    machine_command_destroy(command);

    // Clamping of SUB.
    command = machine_command_create(reg_A, SUB, reg_B, reg_E);
    machine_command_run(command);
    result = machine_result_get();
    assert(result == REGISTER_MAX);
    machine_command_destroy(command);

    command = machine_command_create(reg_A, SUB, reg_D, reg_C);
    machine_command_run(command);
    result = machine_result_get();
    assert(result == REGISTER_MIN);
    machine_command_destroy(command);

    // Clamping of MUL.
    command = machine_command_create(reg_A, MUL, reg_F, reg_G);
    machine_command_run(command);
    result = machine_result_get();
    assert(result == REGISTER_MAX);
    machine_command_destroy(command);

    command = machine_command_create(reg_A, MUL, reg_H, reg_I);
    machine_command_run(command);
    result = machine_result_get();
    assert(result == REGISTER_MIN);
    machine_command_destroy(command);

    // Division by 0.
    command = machine_command_create(reg_A, DIV, reg_J, reg_K);
    machine_command_run(command);
    result = machine_result_get();
    assert(result == data[reg_J]);
    machine_command_destroy(command);
    TEST_END_PRINT();
}

static void test_machine_command_valid_check(void)
{
    TEST_START_PRINT();
    command_t command = {
        .dst = reg_A,
        .op = ADD,
        .src1 = reg_B,
        .src2 = reg_C
    };
    assert(machine_command_valid_check(&command));

    command.dst = -1;
    assert(!machine_command_valid_check(&command));
    command.dst = INT_MAX;
    assert(!machine_command_valid_check(&command));
    command.dst = reg_A;
    assert(machine_command_valid_check(&command));

    command.op = -1;
    assert(!machine_command_valid_check(&command));
    command.op = INT_MAX;
    assert(!machine_command_valid_check(&command));
    command.op = ADD;
    assert(machine_command_valid_check(&command));

    command.src1 = -1;
    assert(!machine_command_valid_check(&command));
    command.src1 = INT_MAX;
    assert(!machine_command_valid_check(&command));
    command.src1 = reg_B;
    assert(machine_command_valid_check(&command));

    command.src2 = -1;
    assert(!machine_command_valid_check(&command));
    command.src2 = INT_MAX;
    assert(!machine_command_valid_check(&command));
    command.src2 = reg_C;
    assert(machine_command_valid_check(&command));
    TEST_END_PRINT();
}
