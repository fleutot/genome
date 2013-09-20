/*----------------------------------------------------------------------------
Copyright (c) 2013 Gauthier Fleutot Östervall
----------------------------------------------------------------------------*/
#ifndef MACHINE_H_INCLUDED
#define MACHINE_H_INCLUDED

#include <stdbool.h>
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

typedef struct {
    register_t dst;
    operation_t op;
    register_t src1;
    register_t src2;
} command_t;

// Signed value allows for easy fair interpretation of register value as
// boolean.
typedef int8_t register_value_t;


//  ----------------------------------------------------------------------------
/// \brief  Initialize the machine's registers to initial data.
/// \param  initial_data Pointer to an array of initial values.
/// \param  nb_initial_regs Number of elements in the array.
/// \return True if no error.
//  ----------------------------------------------------------------------------
bool machine_init(register_value_t *const initial_data,
                  unsigned int const nb_initial_regs);


//  ----------------------------------------------------------------------------
/// \brief  Get the result of the machine.
/// \return Result of the last computation.
//  ----------------------------------------------------------------------------
register_value_t machine_result_get(void);

#endif // MACHINE_H_INCLUDED
