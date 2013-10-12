/*----------------------------------------------------------------------------
Copyright (c) 2013 Gauthier Fleutot Östervall
----------------------------------------------------------------------------*/
#ifndef MACHINE_H_INCLUDED
#define MACHINE_H_INCLUDED

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Type definitions for commands.
typedef enum {
    ADD, SUB, MUL, DIV,
    NB_OPERATION_TYPES  // Must be last.
} operation_t;

typedef enum {
    reg_A, reg_B, reg_C, reg_D, reg_E, reg_F, reg_G, reg_H, reg_I, reg_J, reg_K,
    reg_L, reg_M, reg_N, reg_O, reg_P,
    NB_REGISTERS    // Must be last.
} register_t;

typedef struct command_s command_t;
// Use this instead of sizeof(command_t), since command_t is an imcomplete type.
extern const size_t sizeof_machine_command;

// Signed value allows for easy fair interpretation of register value as
// boolean.
typedef int8_t register_value_t;
#define REGISTER_MAX    (INT8_MAX)
#define REGISTER_MIN    (INT8_MIN)

//  ----------------------------------------------------------------------------
/// \brief  Initialize the machine's registers to initial data.
/// \param  initial_data Pointer to an array of initial values.
/// \param  nb_initial_regs Number of elements in the array.
/// \return True if no error.
//  ----------------------------------------------------------------------------
bool machine_init(register_value_t *const initial_data,
                  unsigned int const nb_initial_regs);

//  ----------------------------------------------------------------------------
/// \brief  Create a new command with the passed parameters as content.
/// \param  out Output register index.
/// \param  op  Operation.
/// \param  in1 Input 1 register index.
/// \param  in2 Input 2 register index.
/// \return Pointer to the newly created command.
//  ----------------------------------------------------------------------------
command_t *machine_command_create(register_t out, operation_t op,
                                  register_t in1,register_t in2);

//  ----------------------------------------------------------------------------
/// \brief  Create a new command with random content.
/// \return Pointer to the newly created command.
/// \pre    rand() is already seeded.
//  ----------------------------------------------------------------------------
command_t *machine_command_random_create(void);

//  ----------------------------------------------------------------------------
/// \brief  Destroy the command passed as parameter.
/// \param  command Pointer to the command to destroy.
//  ----------------------------------------------------------------------------
void machine_command_destroy(command_t *command);

//  ----------------------------------------------------------------------------
/// \brief  Run the command passed as parameter.
/// \param  command Pointer to the command to run.
//  ----------------------------------------------------------------------------
void machine_command_run(command_t const * const command);

//  ----------------------------------------------------------------------------
/// \brief  Print the command passed as parameter.
/// \param  command Pointer to the command to print.
//  ----------------------------------------------------------------------------
void machine_command_print(void const * const command);

//  ----------------------------------------------------------------------------
/// \brief  Check the validity of a command.
/// \param  command Pointer to the command to check.
/// \return True if valid.
//  ----------------------------------------------------------------------------
bool machine_command_valid_check(command_t const * const command);

//  ----------------------------------------------------------------------------
/// \brief  Copy the content of a command to another.
/// \param  dst Destination. Must have memory already allocated.
/// \param  src Source.
//  ----------------------------------------------------------------------------
void machine_command_copy(command_t *dst, command_t *src);

//  ----------------------------------------------------------------------------
/// \brief  Get the result of the machine.
/// \return Result of the last computation.
//  ----------------------------------------------------------------------------
register_value_t machine_result_get(void);

#endif // MACHINE_H_INCLUDED
