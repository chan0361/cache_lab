/*
 * solver.c - Reverse engineer a mystery cache using only its access API.
 *
 * All addresses used by the geometry tests have disjoint physical set-index
 * fields.  In particular, an address stride of 2^(b+9) is a same-set stride
 * for every legal s (1..9).  Thus no geometry test depends on guessing E.
 */
#include "solver.h"

#define MAX_SET_BITS 9
#define POLICY_TRIALS 512

/* Store misses in a no-write-allocate cache do not alter either the data
 * array or replacement metadata, so read hits/misses below are unambiguous
 * even when the write-allocate mode is not known yet. */
static int find_write_allocate(int b)
{
    uint32_t stride = 1u << (b + MAX_SET_BITS);
    cache_flush();
    (void)cache_load(stride);       /* Allocate one known line. */
    (void)cache_store(0);           /* Possibly allocate line zero. */

    /* The final load, rather than the store's hit bit, identifies the mode. */
    return cache_load(0);
}

/* With b and s known, stride is exactly one set-index field.  Hence these
 * are genuinely different tags in one and the same set. */
static int access_tag(int b, int s, unsigned tag)
{
    return cache_load((uint32_t)tag << (b + s));
}

/* Fill every way, then make tag 1 the most recently used line.  A new tag
 * must evict tag 1 under FIFO, but must retain it under LRU.
 * Return 1 exactly when tag 1 was evicted. */
static int replacement_trial(int b, int s, int e)
{
    int i;

    cache_flush();
    for (i = 0; i < e; ++i)
        (void)access_tag(b, s, (unsigned)(i + 1));
    (void)access_tag(b, s, 1); /* Refresh tag 1 for LRU only. */
    (void)access_tag(b, s, (unsigned)(e + 1));
    return !access_tag(b, s, 1);
}

static int find_policy(int b, int s, int e)
{
    int saw_evicted = 0;
    int saw_retained = 0;
    int trial;

    /* Do not reseed or use a different address pattern between repetitions.
     * This is important: independent insertions in one run are what let us
     * observe both possible random eviction outcomes. */
    for (trial = 0; trial < POLICY_TRIALS; ++trial) {
        if (replacement_trial(b, s, e))
            saw_evicted = 1;
        else
            saw_retained = 1;
        if (saw_evicted && saw_retained)
            return POLICY_RANDOM;
    }

    /* A random trial evicts tag 1 with probability 1/e and retains it with
     * probability (e-1)/e.  Misclassification needs 512 copies of the LESS
     * likely outcome.  For each fixed E this probability is at most 2^-512;
     * seeing no mixed outcomes has probability at most twice that.
     * Deterministic LRU always retains; deterministic FIFO always evicts. */
    return saw_evicted ? POLICY_FIFO : POLICY_LRU;
}

void solve(void)
{
    int b = 3;
    int s;
    int e = 1;
    int write_allocate;
    int policy = POLICY_LRU;
    uint32_t universal_stride;

    /* One allocated block is enough to determine its size. */
    cache_flush();
    (void)cache_load(0);
    while (b < 7 && cache_load(1u << b))
        ++b;

    write_allocate = find_write_allocate(b);
    universal_stride = 1u << (b + MAX_SET_BITS);

    /* Find associativity first.  The first n for which an old line misses
     * has n-1 residents immediately before its insertion, hence E=n-1.
     * This holds for LRU, FIFO and every possible random eviction order. */
    for (;;) {
        int i;
        int missed = 0;

        cache_flush();
        for (i = 0; i < e; ++i)
            (void)cache_load((uint32_t)i * universal_stride);
        (void)cache_load((uint32_t)e * universal_stride);
        for (i = 0; i < e; ++i) {
            if (!cache_load((uint32_t)i * universal_stride)) {
                missed = 1;
                break;
            }
        }
        if (missed)
            break;
        ++e; /* At most 16 iterations for every legal cache. */
    }

    /* Now E is known.  For a candidate s below the real value, the E+1
     * lines are distributed among 2^(s_real-s) groups and no group receives
     * more than E lines, so all remain cached.  At the real s (or above),
     * all E+1 lines collide and at least one old line must miss. */
    for (s = 1; s <= MAX_SET_BITS; ++s) {
        int i;
        int missed = 0;

        cache_flush();
        for (i = 0; i <= e; ++i)
            (void)access_tag(b, s, (unsigned)(i + 1));
        for (i = 0; i < e; ++i) {
            if (!access_tag(b, s, (unsigned)(i + 1))) {
                missed = 1;
                break;
            }
        }
        if (missed)
            break;
    }

    if (e > 1)
        policy = find_policy(b, s, e);

    report(b, s, e, policy, write_allocate);
}
