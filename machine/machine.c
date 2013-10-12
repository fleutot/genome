/*----------------------------------------------------------------------------
Copyright (c) 2013 Gauthier Fleutot Östervall
----------------------------------------------------------------------------*/

#include "machine.h"

#include <malloc.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// This must be larger than register_value_t, in order to accomodate for
// operations with boundaries (for example add with ceiling).
typedef int16_t large_register_value_t;

struct command_s {
    register_t dst;
    operation_t op;
    register_t src1;
    register_t src2;
};

//******************************************************************************
// Globals
//******************************************************************************
const size_t sizeof_machine_command = sizeof(command_t);

//******************************************************************************
// Module variables
//******************************************************************************
static register_value_t regs[NB_REGISTERS];


//******************************************************************************
// Function prototypes
//******************************************************************************
static void registers_init(void);
static register_value_t operation_add(register_value_t const a,
                                      register_value_t const b);
static register_value_t operation_sub(register_value_t const a,
                                      register_value_t const b);
static register_value_t operation_mul(register_value_t const a,
                                      register_value_t const b);
static register_value_t operation_div(register_value_t const a,
                                      register_value_t const b);
static large_register_value_t clamp(large_register_value_t const value);


//******************************************************************************
// Module constants
//******************************************************************************
register_value_t (*operation[NB_OPERATION_TYPES]) (register_value_t const a,
                                                   register_value_t const b) = {
    [ADD] = operation_add,
    [SUB] = operation_sub,
    [MUL] = operation_mul,
    [DIV] = operation_div
};


//******************************************************************************
// Function definitions
//******************************************************************************
//  ----------------------------------------------------------------------------
/// \brief  Initialize the machine's registers to the data passed as
/// parameter. Default to zero if not all registers are given a value.
/// \param  initial_data Pointer to an array of initial values.
/// \param  nb_initial_regs Number of elements in the array.
/// \return True if the number of init values was not too large.
//  ----------------------------------------------------------------------------
bool machine_init(register_value_t *const initial_data,
                  unsigned int const nb_initial_regs)
{
    if (nb_initial_regs > NB_REGISTERS) {
        return false;
    }

    registers_init();   // Default init.
    for (unsigned int i = 0; i < nb_initial_regs; i++) {
        regs[i] = initial_data[i];
    }
    return true;
}


//  ----------------------------------------------------------------------------
/// \brief  Create a new object command and return an address to it.
/// \param  out The output register index.
/// \param  op  The operation to execute.
/// \param  in1 The first operand of op.
/// \param  in2 The second operand of op.
/// \return Pointer to the created command.
//  ----------------------------------------------------------------------------
command_t *machine_command_create(register_t out, operation_t op,
                                  register_t in1,register_t in2)
{
    command_t *new_command = malloc(sizeof(command_t));
    if (new_command == NULL) {
        fprintf(stderr, "%s: new_command is NULL.\n", __func__);
        return NULL;
    }

    *new_command = (command_t) {
        .dst = out,
        .op = op,
        .src1 = in1,
        .src2 = in2
    };

    return new_command;
}


//  ----------------------------------------------------------------------------
/// \brief  Create a new object command with random content (registers and
/// operation).
/// \return Pointer to the created command.
/// \pre    The random number generator is already seeded.
//  ----------------------------------------------------------------------------
command_t *machine_command_random_create(void)
{
    // Modulo of a random number with a number that is not a power of two is
    // no uniformly random, but that is not very important for this
    // application.
    register_t dst = (register_t) rand() % NB_REGISTERS;
    operation_t op  = (operation_t) rand() % NB_OPERATION_TYPES;
    register_t src1 = (register_t) rand() % NB_REGISTERS;
    register_t src2 = (register_t) rand() % NB_REGISTERS;

    command_t *new_command_p = machine_command_create(dst, op, src1, src2);
    return new_command_p;
}


//  ----------------------------------------------------------------------------
/// \brief  Free the memory allocated for command.
/// \param  command Pointer to the command object to free.
//  ----------------------------------------------------------------------------
void machine_command_destroy(command_t *command)
{
    free(command);
}


//  ----------------------------------------------------------------------------
/// \brief  Run the command passed as parameter.
/// \param  command Pointer to the command to run.
//  ----------------------------------------------------------------------------
void machine_command_run(command_t const * const command)
{
    if (command == NULL) {
        fprintf(stderr, "%s, command is NULL.\n", __func__);
        return;
    }

    if (command->op >= NB_OPERATION_TYPES
        || command->dst >= NB_REGISTERS
        || command->src1 >= NB_REGISTERS
        || command->src2 >= NB_REGISTERS
        ) {
        fprintf(stderr, "%s, command is erroneous.\n", __func__);
        machine_command_print(command);
        return;
    }

    regs[command->dst] = operation[command->op](regs[command->src1],
                                                regs[command->src2]);
}


//  ----------------------------------------------------------------------------
/// \brief  Print the command passed as parameter in a format that is quite
/// human readable. Only the index to registers and to the operation are
/// printed.
/// \param  command Pointer to the command to print. void pointer so that the
/// function can be used as a callback.
//  ----------------------------------------------------------------------------
void machine_command_print(void const * const command)
{
    static const char *reg_to_string[NB_REGISTERS] = {
        [reg_A] = "A", [reg_B] = "B", [reg_C] = "C", [reg_D] = "D",
        [reg_E] = "E", [reg_F] = "F", [reg_G] = "G", [reg_H] = "H",
        [reg_I] = "I", [reg_J] = "J", [reg_K] = "K", [reg_L] = "L",
        [reg_M] = "M", [reg_N] = "N", [reg_O] = "O", [reg_P] = "P"
    };

    static const char *op_to_string[NB_OPERATION_TYPES] = {
        [ADD] = "+", [SUB] = "-", [MUL] = "*", [DIV] = "/"
    };

    command_t *cmd = (command_t *) command;

    printf("%s = (%s %s %s)\n",
           reg_to_string[cmd->dst],
           op_to_string[cmd->op],
           reg_to_string[cmd->src1],
           reg_to_string[cmd->src2]
        );
}


//  ----------------------------------------------------------------------------
/// \brief  Check the validity of a command.
/// \param  command Pointer to the command to check.
/// \return True if valid.
//  ----------------------------------------------------------------------------
bool machine_command_valid_check(command_t const * const command)
{
    if (command->dst < 0 || command->dst >= NB_REGISTERS
        || command->op < 0 || command->op >= NB_OPERATION_TYPES
        || command->src1 < 0 || command->src1 >= NB_REGISTERS
        || command->src2 < 0 || command->src2 >= NB_REGISTERS
        ) {
        return false;
    } else {
        return true;
    }
}

//  ----------------------------------------------------------------------------
/// \brief  Copy a command into another, discarding the original content.
/// \param  dst
/// \param  src
//  ----------------------------------------------------------------------------
void machine_command_copy(command_t *dst, command_t *src)
{
    assert(dst);
    assert(src);
    memcpy(dst, src, sizeof(command_t));
}


//  ----------------------------------------------------------------------------
/// \brief  The result may be placed in whichever register after running the
/// machine.
/// \return The value in the register that was chosen as output register.
//  ----------------------------------------------------------------------------
register_value_t machine_result_get(void)
{
    return regs[reg_A];
}


//******************************************************************************
// Internal functions
//******************************************************************************
static void registers_init(void)
{
    for (int i = 0; i < NB_REGISTERS; i++) {
        regs[i] = 0;
    }
}

//  ----------------------------------------------------------------------------
/// \brief  Definition of command operations.
//  ----------------------------------------------------------------------------
static register_value_t operation_add(register_value_t const a,
                                      register_value_t const b)
{
    large_register_value_t result = (large_register_value_t) a + b;
    result = clamp(result);
    return (register_value_t) result;
}


static register_value_t operation_sub(register_value_t const a,
                                      register_value_t const b)
{
    large_register_value_t result = (large_register_value_t) a - b;
    result = clamp(result);
    return (register_value_t) result;
}


static register_value_t operation_mul(register_value_t const a,
                                      register_value_t const b)
{
    large_register_value_t result = (large_register_value_t) a * b;
    result = clamp(result);
    return (register_value_t) result;
}


static register_value_t operation_div(register_value_t const a,
                                      register_value_t const b)
{
    if (b == 0) {
        return a;
    }
    return a / b;
}

static large_register_value_t clamp(large_register_value_t const value)
{
    if (value > REGISTER_MAX) {
        return REGISTER_MAX;
    } else if (value < REGISTER_MIN) {
        return REGISTER_MIN;
    }

    return value;
}
