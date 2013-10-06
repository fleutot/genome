/*----------------------------------------------------------------------------
Copyright (c) 2013 Gauthier Fleutot Östervall
----------------------------------------------------------------------------*/
#include "genome.h"

#include <malloc.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "linkedlist/linkedlist.h"
#include "machine/machine.h"


//******************************************************************************
// Type definitions
//******************************************************************************
struct genome_s {
    linkedlist_t *genes;
};

// This typedef sets the max size of the genome at creation.
// uint8_t gives max size 255.
// Genomes can grow further by breeding.
typedef uint8_t genome_start_size_t;

//******************************************************************************
// Module constants
//******************************************************************************

//******************************************************************************
// Module variables
//******************************************************************************
// Do not seed more than once.
static bool randomizer_seeded = false;
// Must be initialized to true before checking for genome validity.
static bool gene_was_valid = false;

//******************************************************************************
// Function prototypes
//******************************************************************************
static void gene_display(void const * const data);
static void gene_valid_check(void const * const data);

//******************************************************************************
// Function definitions
//******************************************************************************
//  ----------------------------------------------------------------------------
/// \brief  Create a genome with random commands.
/// \return Pointer to the newly allocated genome.
//  ----------------------------------------------------------------------------
genome_t *genome_random_create(void)
{
    genome_t *new_genome_p = genome_create();
    if (new_genome_p == NULL) {
        fprintf(stderr, "%s: new_genome_p is NULL.\n", __func__);
        return NULL;
    }

    if (!randomizer_seeded) {
        // Seed only once.
        srand(time(NULL));
        randomizer_seeded = true;
    }

    // Implicit type conversion limits the result.
    genome_start_size_t genome_size = rand();

    for (genome_start_size_t i = 0; i < genome_size; i++) {
        command_t *new_command_p = machine_command_random_create();
        if (new_command_p == NULL) {
            fprintf(stderr, "%s: new_command_p is NULL.\n", __func__);
            genome_destroy(&new_genome_p);
            return NULL;
        }
        linkedlist_add(new_genome_p->genes, new_command_p);
    }

    if (!genome_sanity_check(new_genome_p)) {
        fprintf(stderr, "%s: new genome is corrupt.\n", __func__);
    }

    return new_genome_p;
}


//  ----------------------------------------------------------------------------
/// \brief  Create a new genome object, including a linkedlist object (no data).
/// \return Pointer to the newly created genome object.
//  ----------------------------------------------------------------------------
genome_t *genome_create(void)
{
    genome_t *new_genome_p = malloc(sizeof (genome_t));
    if (new_genome_p == NULL) {
        fprintf(stderr, "%s: new_genome_p is NULL.\n", __func__);
        return NULL;
    }

    new_genome_p->genes = linkedlist_create();
    if (new_genome_p->genes == NULL) {
        fprintf(stderr, "%s: linked list is NULL.\n", __func__);
        free(new_genome_p);
        return NULL;
    }
    return new_genome_p;
}

//  ----------------------------------------------------------------------------
/// \brief  Check if the genes of genome look right, by looking at the data
/// itself. So far this only checks if the elements of command are in range.
/// \param  genome Pointer to the genome to check.
/// \return True if the genom seems valid.
//  ----------------------------------------------------------------------------
bool genome_sanity_check(genome_t const * const genome)
{
    if (genome == NULL) {
        fprintf(stderr, "%s: genome is NULL.\n", __func__);
        return false;
    }
    gene_was_valid = true;
    linkedlist_run_for_all(genome->genes, gene_valid_check);
    return gene_was_valid;
}


//  ----------------------------------------------------------------------------
/// \brief  Copy a genome to another. Data objects are duplicated.
/// \param  dst
/// \param  src
//  ----------------------------------------------------------------------------
void genome_copy(genome_t ** const dst, genome_t const * const src)
{
    if (src == NULL) {
        fprintf(stderr, "%s: src is NULL.\n", __func__);
        return;
    }

    if (*dst != NULL) {
        genome_destroy(dst);
    }
    *dst = genome_create();
    linkedlist_copy((*dst)->genes, src->genes, sizeof_machine_command);
}


//  ----------------------------------------------------------------------------
/// \brief  Make two offsprings of two parents. The offsprings are first copies
/// of the parents, then portions of genomes are swapped from one offspring to
/// the other, then some mutations on the genes may occur.
/// \param  offspring1 Pointer to pointer to the first new offspring.
/// \param  offspring2 Pointer to pointer to the secod new offspring.
/// \param  parent1 Pointer to the first parent.
/// \param  parent2 Pointer to the second parent.
//  ----------------------------------------------------------------------------
void genome_breed(genome_t ** const offspring1, genome_t ** const offspring2,
                  genome_t const * const parent1,
                  genome_t const * const parent2)
{
    if (parent1 == NULL || parent2 == NULL) {
        fprintf(stderr, "%s: at least a parent is NULL.\n", __func__);
        return;
    }

    genome_copy(offspring1, parent1);
    genome_copy(offspring2, parent2);
}


//  ----------------------------------------------------------------------------
/// \brief  Free the memory allocated for genome. All commands are deallocated,
/// so it will break any other genome sharing data elements (don't share data
/// elements, copy them).
/// \param  genome The genome to free.
//  ----------------------------------------------------------------------------
void genome_destroy(genome_t **genome)
{
    if ((genome == NULL) || (*genome == NULL)) {
        fprintf(stderr, "%s: genome is NULL.\n", __func__);
    } else {
        linkedlist_destroy((*genome)->genes);
        free(*genome);
        // When making e.g. copy of genomes, if the destination is already
        // allocated it needs to be freed. Assign NULL to flag that there is no
        // memory allocated for this genome anymore.
        *genome = NULL;
    }
}


//******************************************************************************
// Internal functions
//******************************************************************************
//  ----------------------------------------------------------------------------
/// \brief  Check if the gene passed as parameter seems valid. So far it just
/// checks if operation and operands are in range. To be used as callback to
/// linkedlist_run_for_all().
/// \param  data Pointer to the gene to check.
/// \return The result is stored in the module variable gene_was_valid, which
/// needs to be reset to true before checking a new genome.
//  ----------------------------------------------------------------------------
static void gene_valid_check(void const * const data)
{
    command_t *command = (command_t *) data;

    if (!machine_command_valid_check(command)) {
        fprintf(stderr, "%s: invalid check.\n", __func__);
        gene_was_valid = false;
    }
}
