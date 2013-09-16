/*----------------------------------------------------------------------------
Copyright (c) 2013 Gauthier Fleutot Östervall
----------------------------------------------------------------------------*/

#ifndef GENOME_H_INCLUDED
#define GENOME_H_INCLUDED

#include <stdbool.h>

typedef struct genome_s genome_t;


//  ----------------------------------------------------------------------------
/// \brief  Create a new genome of random size and random genes.
/// \return Pointer to the new random genome.
//  ----------------------------------------------------------------------------
genome_t *genome_random_create(void);


//  ----------------------------------------------------------------------------
/// \brief  Free the memory allocated for genome.
/// \param  genome The genome to free.
//  ----------------------------------------------------------------------------
void genome_destroy(genome_t *genome);


//  ----------------------------------------------------------------------------
/// \brief  Check if the genome passed as parameter looks right.
/// \param  True if the genome seems valid.
//  ----------------------------------------------------------------------------
bool genome_sanity_check(genome_t const * const genome);


#endif // GENOME_H_INCLUDED
