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
/// \brief  Create a new empty genome.
/// \return Pointer to the newly created genome.
//  ----------------------------------------------------------------------------
genome_t *genome_create(void);

//  ----------------------------------------------------------------------------
/// \brief  Free the memory allocated for genome.
/// \param  genome The genome to free (pointer to pointer, sets to NULL).
//  ----------------------------------------------------------------------------
void genome_destroy(genome_t **genome);

//  ----------------------------------------------------------------------------
/// \brief  Check if the genome passed as parameter looks right.
/// \param  True if the genome seems valid.
//  ----------------------------------------------------------------------------
bool genome_sanity_check(genome_t const * const genome);

//  ----------------------------------------------------------------------------
/// \brief  Copy a genome to another.
/// \param  dst Destination genome. First destroyed if not already empty.
/// \param  src Source genome.
//  ----------------------------------------------------------------------------
void genome_copy(genome_t ** const dst, genome_t const * const src);

//  ----------------------------------------------------------------------------
/// \brief  Print out the size and all data in the genome.
/// \param  genome  Genome of which the genes are to be displayed.
//  ----------------------------------------------------------------------------
void genome_display(genome_t const * const genome);

//  ----------------------------------------------------------------------------
/// \brief  Make two offsprings of two parents.
/// \param  offspring1 Pointer to pointer to the first new offspring.
/// \param  offspring2 Pointer to pointer to the secod new offspring.
/// \param  parent1 Pointer to the first parent.
/// \param  parent2 Pointer to the second parent.
//  ----------------------------------------------------------------------------
void genome_breed(genome_t ** const offspring1, genome_t ** const offspring2,
                  genome_t const * const parent1,
                  genome_t const * const parent2);

#endif // GENOME_H_INCLUDED
