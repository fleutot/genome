/*----------------------------------------------------------------------------
Copyright (c) 2013 Gauthier Fleutot Östervall
----------------------------------------------------------------------------*/

#include "machine.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>


//******************************************************************************
// Module constants
//******************************************************************************

//******************************************************************************
// Module variables
//******************************************************************************
static register_value_t regs[NB_REGISTERS];


//******************************************************************************
// Function prototypes
//******************************************************************************
static void registers_init(void);


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
