/*----------------------------------------------------------------------------
Copyright (c) 2013 Gauthier Fleutot Östervall
----------------------------------------------------------------------------*/
#include "randomizer.h"

#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

//******************************************************************************
// Module variables
//******************************************************************************
static bool randomizer_seeded = false;

//******************************************************************************
// Function definitions
//******************************************************************************
int random_get(const int limit)
{
    if (!randomizer_seeded) {
        srandom(time(NULL));
        randomizer_seeded = true;
    }

    // This does not give real uniformity, but since random returns long int and
    // limit is not expect to be very large, it is enough.
    return (int) (random() % limit);
}

