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
typedef uint8_t genome_size_t;


//******************************************************************************
// Module constants
//******************************************************************************
#define GENOME_DEBUG

//******************************************************************************
// Module variables
//******************************************************************************
static bool randomizer_seeded = false;


//******************************************************************************
// Function prototypes
//******************************************************************************
void gene_destroy(void const * const data);


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

    // These uses of rand() are somewhat skewed (non uniform). It is ok in this
    // application, since the sizes of the data types are small and the skew is
    // small.
    genome_size_t genome_size = rand();

    #if defined(GENOME_DEBUG)
    printf("Size of new genome: %d\n", genome_size);
    #endif // GENOME_DEBUG

    for (genome_size_t i = 0; i < genome_size; i++) {
        command_t *new_command_p = malloc(sizeof (command_t));
        new_command_p->dst = (register_t) rand();
        new_command_p->op  = (operation_t) rand();
        new_command_p->src1 = (register_t) rand();
        new_command_p->src2 = (register_t) rand();
        linkedlist_add(new_genome_p->genes, new_command_p);
    }

    return new_genome_p;
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
void gene_destroy(void const * const data)
{
    // This is deliberate, killing data in spite of constness.  Other callbacks
    // to linkedlist should not touch data, destroy is an exception. This may
    // change in the future, to allow for genetic mutations for example.
    free((void *) data);
}
