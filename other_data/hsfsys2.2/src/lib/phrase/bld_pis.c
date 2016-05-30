/*
# proc: build_pi_lists - builds lists of phrase indices referencing segmented
# proc:                  characters sorted into reading order based on
# proc:                  segmentor statistics.
# proc: build_pi_lists_Rel2 - builds lists of phrase indices referencing segmented
# proc:                  characters sorted into reading order based on
# proc:                  segmentor statistics using dynamic allocation.
*/

#include <stdio.h>
#include <phrase.h>

#define A_LIMIT      300.0
#define PA1            0.375
#define A1            12.0
#define B1             0.1
#define PA2            4.7
#define A2           150.0
#define B2             2.0
#define PREV_MEAN_FACTOR  1.0
#define MEAN_FACTOR    0.25
#define SLOPE_THRESH   0.25
#define MRG_FACTOR     0.75
#define MRG2_FACTOR     0.50
#define RUN_FACTOR     0.1

/*************************************************************/
build_pi_lists(pi_lists, pi_lens, nphrases, xs, ys, ws, hs, n)
int *pi_lists[], *pi_lens;
int *nphrases;
int *xs, *ys, *ws, *hs;
int n;
{
   float hmean, compute_mean();
   int *mxs, *mys;

   malloc_int(&mxs, n, "build_pi_lists : mxs");
   malloc_int(&mys, n, "build_pi_lists : mys");
   compute_mxs_mys(xs, ys, ws, hs, mxs, mys, n);

   *nphrases = 0;
   hmean = compute_mean(hs, n);
   find_pi_lists(pi_lists, pi_lens, nphrases,
                 hmean, PREV_MEAN_FACTOR, MEAN_FACTOR, SLOPE_THRESH,
                 mxs, mys, n, PA1, B1, PA2, B2, A1, A_LIMIT);
   merge_pi_lists(pi_lists, pi_lens, nphrases, mxs, mys, n, hmean,
                 MRG_FACTOR, MRG2_FACTOR, RUN_FACTOR);
   sort_pi_lists_on_y(pi_lists, pi_lens, *nphrases, mys, n);
   free(mxs);
   free(mys);
}

/*************************************************************/
build_pi_lists_Rel2(pi_lists, pi_lens, nphrases, problems, nprob, hmean,
                    mxs, mys, n)
int ***pi_lists, **pi_lens, **problems;
int *nphrases, *nprob;
int *mxs, *mys, n;
float hmean;
{
   int *pi_alens, aphrases, aprob;

   *nphrases = 0;
   aphrases = PHRASE_NUM_CHUNKS;
   malloc_dbl_int_l1(pi_lists, aphrases, "build_pi_lists_Rel2 : pi_lists");
   malloc_int(&pi_alens, aphrases,  "build_pi_lists_Rel2 : pi_alens");
   malloc_int(pi_lens, aphrases,  "build_pi_lists_Rel2 : pi_lens");
   *nprob = 0;
   aprob = PHRASE_LEN_CHUNKS;
   malloc_int(problems, aprob,  "build_pi_lists_Rel2 : problems");

   find_pi_lists_Rel2(pi_lists, pi_lens, &pi_alens, nphrases, &aphrases,
                  problems, nprob, &aprob,
                  hmean, PREV_MEAN_FACTOR, MEAN_FACTOR, SLOPE_THRESH,
                  mxs, mys, n, PA1, B1, PA2, B2, A1, A_LIMIT);
   merge_pi_lists_Rel2((*pi_lists), (*pi_lens), nphrases, mxs, mys, n, hmean,
                  MRG_FACTOR, MRG2_FACTOR, RUN_FACTOR);

   sort_pi_lists_on_y((*pi_lists), (*pi_lens), *nphrases, mys, n);
   free(pi_alens);
}
