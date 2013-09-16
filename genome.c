/*----------------------------------------------------------------------------
Copyright (c) 2013 Gauthier Fleutot Östervall
----------------------------------------------------------------------------*/
#include "genome.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "linkedlist/linkedlist.h"

struct genome_s {
    linkedlist_t *genes;
};

typedef enum {
    ADD, SUB, MUL, DIV,
    NB_OPERATION_TYPES  // Must be last.
} operation_t;

typedef enum {
    A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P,
    NB_REGISTERS    // Must be last.
} register_t;

typedef struct {
    register_t dst;
    operation_t op;
    register_t src1;
    register_t src2;
} command_t;

// This typedef sets the max size of the genome. uint16_t sets the max to 65535,
// for example. uint8_t sets the max to 255.
// It is not currently possible to have a max that is not a power of two (not
// having a power of two would result in a skew in the random distribution. Not
// a big problem, but still).
typedef uint8_t genome_size_t;


//******************************************************************************
// Module constants
//******************************************************************************
#define GENOME_DEBUG

//******************************************************************************
// Module variables
//******************************************************************************
static bool randomizer_seeded = false;
static bool gene_was_valid = false;


//******************************************************************************
// Function prototypes
//******************************************************************************
static void gene_destroy(void const * const data);
static void gene_display(void const * const data);
static void gene_valid_check(void const * const data);


//******************************************************************************
// Function definitions
//******************************************************************************
//  ----------------------------------------------------------------------------
/// \brief  Create a genome with random commands.
/// \return Pointer to the newly allocated genom.
//  ----------------------------------------------------------------------------
genome_t *genome_random_create(void)
{
    genome_t *new_genome_p = malloc(sizeof (genome_t));
    if (new_genome_p == NULL) {
        fprintf(stderr, "%s: new_genome_p is NULL.\n", __func__);
        return NULL;
    }
    new_genome_p->genes = linkedlist_create();

    if (!randomizer_seeded) {
        srand(time(NULL));
        randomizer_seeded = true;
    }

    genome_size_t genome_size = rand();

    for (genome_size_t i = 0; i < genome_size; i++) {
        command_t *new_command_p = malloc(sizeof (command_t));
        // Modulo of a random number with a number that is not a power of two is
        // no uniformly random, but that is not very important for this
        // application.
        new_command_p->dst = (register_t) rand() % NB_REGISTERS;
        new_command_p->op  = (operation_t) rand() % NB_OPERATION_TYPES;
        new_command_p->src1 = (register_t) rand() % NB_REGISTERS;
        new_command_p->src2 = (register_t) rand() % NB_REGISTERS;
        linkedlist_add(new_genome_p->genes, new_command_p);
    }

    if (!genome_sanity_check(new_genome_p)) {
        fprintf(stderr, "%s: new genome is corrupt.\n", __func__);
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
    gene_was_valid = true;
    linkedlist_run_for_all(genome->genes, gene_valid_check);
    return gene_was_valid;
}


//  ----------------------------------------------------------------------------
/// \brief  Free the memory allocated for genome. All commands are deallocated,
/// so it will break any other genome sharing data elements (don't share data
/// elements, copy them).
/// \param  genome The genome to free.
//  ----------------------------------------------------------------------------
void genome_destroy(genome_t *genome)
{
    if (genome == NULL) {
        fprintf(stderr, "%s: genome is NULL.\n", __func__);
    } else {
        // The linked list does not own the data, freeing of the commands must
        // happen here.
        linkedlist_run_for_all(genome->genes, gene_destroy);
        linkedlist_destroy(genome->genes);
        free(genome);
    }
}


//******************************************************************************
// Internal functions
//******************************************************************************
//  ----------------------------------------------------------------------------
/// \brief  Deallocate the data for a gene. This must be done here, since the
/// linkedlist used to sequence the genes does not own the genes themselves. To
/// be used as callback to linkedlist_run_for_all().
/// \param  data Pointer to the gene to deallocate.
//  ----------------------------------------------------------------------------
static void gene_destroy(void const * const data)
{
    // The typecast is deliberate, killing data in spite of constness.  Other
    // callbacks to linkedlist should not touch data, destroy is an
    // exception. This may change in the future, to allow for genetic mutations
    // for example.
    free((void *) data);
}


//  ----------------------------------------------------------------------------
/// \brief  Display the command gene passed as parameter on standard output. To
/// be used as callback to linkedlist_run_for_all().
/// \param  data Pointer to the gene to display.
//  ----------------------------------------------------------------------------
static void gene_display(void const * const data)
{
    static const char *reg_to_string[NB_REGISTERS] = {
        [A] = "A", [B] = "B", [C] = "C", [D] = "D", [E] = "E", [F] = "F",
        [G] = "G", [H] = "H", [I] = "I", [J] = "J", [K] = "K", [L] = "L",
        [M] = "M", [N] = "N", [O] = "O", [P] = "P"
    };

    static const char *op_to_string[NB_OPERATION_TYPES] = {
        [ADD] = "+", [SUB] = "-", [MUL] = "*", [DIV] = "/"
    };

    command_t *command = (command_t *) data;

    #if defined(GENOME_DEBUG)
    gene_was_valid = true;
    gene_valid_check(data);
    if (!gene_was_valid) {
        fprintf(stderr, "%s: gene member out of range.\n", __func__);
        return;
    }
    #endif // GENOME_DEBUG

    printf("gene %s = (%s %s %s)\n",
           reg_to_string[command->dst],
           op_to_string[command->op],
           reg_to_string[command->src1],
           reg_to_string[command->src2]
        );
}


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

    if (command->dst < 0 || command->dst >= NB_REGISTERS
        || command->op < 0 || command->op >= NB_OPERATION_TYPES
        || command->src1 < 0 || command->src1 >= NB_REGISTERS
        || command->src2 < 0 || command->src2 >= NB_REGISTERS
        ) {
        gene_was_valid = false;
    }
}
