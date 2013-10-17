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
/// \brief  Get the size of a genome.
/// \param  genome  Pointer to the genome.
/// \return The size of the genome.
//  ----------------------------------------------------------------------------
int genome_size_get(genome_t const * const genome);

//  ----------------------------------------------------------------------------
/// \brief  Compare two genomes
/// \param  gen1
/// \param  gen2
/// \return True if the genomes were equal.
//  ----------------------------------------------------------------------------
bool genome_compare(genome_t * const gen1, genome_t * const gen2);


//  ----------------------------------------------------------------------------
/// \brief  Print out the size and all data in the genome.
/// \param  genome  Genome of which the genes are to be displayed.
//  ----------------------------------------------------------------------------
void genome_display(genome_t const * const genome);


//  ----------------------------------------------------------------------------
/// \brief  Crossover two genomes, resulting in a blend of the two
/// originals. Two random parts of the genomes are swapped.
/// \param  genome1 First genome to blend.
/// \param  genome2 Second genome to blend.
//  ----------------------------------------------------------------------------
void genome_crossover(genome_t const * const genome1,
                      genome_t const * const genome2);

//  ----------------------------------------------------------------------------
/// \brief  Mutate a genome. Take a random gene and replace it by a randomly
/// generated one.
/// \param  genome  The genome to mutate.
//  ----------------------------------------------------------------------------
void genome_mutate(genome_t const * const genome);

#endif // GENOME_H_INCLUDED
