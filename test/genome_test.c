/*----------------------------------------------------------------------------
Copyright (c) 2013 Gauthier Fleutot Östervall
----------------------------------------------------------------------------*/

// Module under test.
#include "../genome.h"

#include <assert.h>
#include <stddef.h>
#include <stdio.h>


//******************************************************************************
// Module constants
//******************************************************************************

//******************************************************************************
// Module variables
//******************************************************************************

//******************************************************************************
// Function prototypes
//******************************************************************************
// Test functions.
static void test_genome_random_create(void);
static void test_genome_copy(void);
static void test_genome_breed(void);

//******************************************************************************
// Function definitions
//******************************************************************************
int main(void)
{
    test_genome_random_create();
    test_genome_copy();
    test_genome_breed();
    printf("All tests passed.\n");
}


//******************************************************************************
// Internal functions
//******************************************************************************
static void test_genome_random_create(void)
{
    fprintf(stdout, "Running %s...", __func__); fflush(stdout);

    genome_t *genome = genome_random_create();

    assert(genome != NULL);
    // It is hard to test for randomness.

    genome_destroy(&genome);
    fprintf(stdout, "OK\n");
}


static void test_genome_copy(void)
{
    fprintf(stdout, "Running %s...", __func__); fflush(stdout);
    genome_t *src1 = genome_random_create();
    genome_t *src2 = genome_random_create();
    genome_t *dst = NULL;

    genome_copy(&dst, src1);
    assert(genome_sanity_check(dst));

    // Test overwriting.
    genome_copy(&dst, src2);
    assert(genome_sanity_check(dst));

    genome_destroy(&dst);
    genome_destroy(&src1);
    genome_destroy(&src2);

    fprintf(stdout, "OK\n");
}


static void test_genome_breed(void)
{
    fprintf(stdout, "Running %s...", __func__); fflush(stdout);

    genome_t *genome1 = genome_random_create();
    genome_t *genome2 = genome_random_create();

    genome_t *offspring1 = NULL;
    genome_t *offspring2 = NULL;

    genome_breed(&offspring1, &offspring2, genome1, genome2);

    assert(genome_sanity_check(offspring1));
    assert(genome_sanity_check(offspring2));

    // Retest with existing offsprings.
    genome_breed(&offspring1, &offspring2, genome1, genome2);

    assert(genome_sanity_check(offspring1));
    assert(genome_sanity_check(offspring2));

    // Need a more interesting test! How to control that the breed is ok?

    genome_destroy(&genome1);
    genome_destroy(&genome2);
    genome_destroy(&offspring1);
    genome_destroy(&offspring2);

    fprintf(stdout, "OK\n");
}
