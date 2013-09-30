/*----------------------------------------------------------------------------
Copyright (c) 2013 Gauthier Fleutot Östervall
----------------------------------------------------------------------------*/
// Module under test.
#include "../machine.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

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


//******************************************************************************
// Function definitions
//******************************************************************************
int main(void)
{
    test_machine_init();
    test_machine_command_create();
    test_machine_command_with_clamp();
    printf("All tests passed.\n");
}


//******************************************************************************
// Internal functions
//******************************************************************************
static void test_machine_init(void)
{
    register_value_t data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
                               15,16, 17};

    bool init_result;

    init_result = machine_init(data, NB_REGISTERS + 1);
    assert(init_result == false);

    init_result = machine_init(data, NB_REGISTERS);
    assert(init_result == true);

    assert(machine_result_get() == data[0]);
}

static void test_machine_command_create(void)
{
    register_value_t data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
                               15,16, 17};

    command_t *command = machine_command_create(reg_A, ADD, reg_B, reg_C);

    machine_init(data, NB_REGISTERS);
    machine_command_run(command);
    register_value_t result = machine_result_get();

    assert(result == data[1] + data[2]);

    machine_command_destroy(command);
}


static void test_machine_command_with_clamp(void)
{
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
}
