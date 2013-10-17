/*----------------------------------------------------------------------------
Copyright (c) 2013 Gauthier Fleutot Östervall
----------------------------------------------------------------------------*/

// Module under test.
#include "../genome.h"

#include <assert.h>
#include <stddef.h>
#include <stdio.h>


//******************************************************************************
// Module macros
//******************************************************************************
#define TEST_START_PRINT()    do {              \
        printf("Running %s...", __func__);      \
        fflush(stdout);                         \
    } while (0)

#define TEST_END_PRINT()  do {                  \
        printf("OK.\n");                        \
    } while (0)

//******************************************************************************
// Module variables
//******************************************************************************

//******************************************************************************
// Function prototypes
//******************************************************************************
// Test functions.
static void test_genome_random_create(void);
static void test_genome_copy(void);
static void test_genome_crossover(void);
static void test_genome_mutate(void);
static void test_genome_compare(void);

//******************************************************************************
// Function definitions
//******************************************************************************
int main(void)
{
    test_genome_random_create();
    test_genome_copy();
    test_genome_crossover();
    test_genome_compare();
    test_genome_mutate();
    printf("All tests passed.\n");
}


//******************************************************************************
// Internal functions
//******************************************************************************
static void test_genome_random_create(void)
{
    TEST_START_PRINT();

    genome_t *genome = genome_random_create();

    assert(genome != NULL);

    // It is hard to test for randomness. Uncomment and have a look.
    //genome_display(genome);

    genome_destroy(&genome);
    TEST_END_PRINT();
}


static void test_genome_copy(void)
{
    TEST_START_PRINT();
    genome_t *src1 = genome_random_create();
    genome_t *src2 = genome_random_create();
    genome_t *dst = NULL;

    genome_copy(&dst, src1);
    assert(genome_sanity_check(dst));
    assert(genome_size_get(dst) == genome_size_get(src1));

    // Test overwriting.
    genome_copy(&dst, src2);
    assert(genome_sanity_check(dst));
    assert(genome_size_get(dst) == genome_size_get(src2));

    genome_destroy(&dst);
    genome_destroy(&src1);
    genome_destroy(&src2);

    TEST_END_PRINT();
}


static void test_genome_crossover(void)
{
    TEST_START_PRINT();

    genome_t *genome1 = genome_random_create();
    genome_t *genome2 = genome_random_create();

    genome_t *offspring1 = NULL;
    genome_t *offspring2 = NULL;

    genome_copy(&offspring1, genome1);
    genome_copy(&offspring2, genome2);

    genome_crossover(offspring1, offspring2);

    assert(genome_sanity_check(offspring1));
    assert(genome_sanity_check(offspring2));
    assert((genome_size_get(genome1) + genome_size_get(genome2))
           == (genome_size_get(offspring1) + genome_size_get(offspring2)));

    genome_destroy(&genome1);
    genome_destroy(&genome2);
    genome_destroy(&offspring1);
    genome_destroy(&offspring2);

    TEST_END_PRINT();
}


static void test_genome_compare(void)
{
    TEST_START_PRINT();
    genome_t *genome1 = genome_random_create();
    genome_t *genome2 = genome_random_create();

    assert(!genome_compare(genome1, genome2));

    genome_t *genome1_copy = genome_create();
    genome_copy(&genome1_copy, genome1);
    assert(genome_compare(genome1, genome1_copy));

    genome_destroy(&genome1);
    genome_destroy(&genome2);
    genome_destroy(&genome1_copy);

    TEST_END_PRINT();
}


static void test_genome_mutate(void)
{
    TEST_START_PRINT();

    genome_t *origin = genome_random_create();
    genome_t *mutant = genome_create();

    genome_copy(&mutant, origin);

    assert(genome_compare(mutant, origin));

    genome_mutate(mutant);

    assert(!genome_compare(mutant, origin));

    genome_destroy(&origin);
    genome_destroy(&mutant);

    TEST_END_PRINT();
}
