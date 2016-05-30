/*
# proc: lookup_levenstein - given a word string, mfs structure of words or
# proc:                     phrases, determines which mfs entry is the best
# proc:                     match to the word using the Lenenstein Distance.
# proc: init_align_params - initializes the global parameter structure
# proc:                     controlling the Levenstein Distance algorithm.
# proc: end_lookup_levenstein - deallocates the global parameter structure.
# proc:
*/

#include <stdio.h>
#include <mfs.h>
#include <align.h>

#define DEF_SUB_PENALTY 1
#define DEF_DEL_PENALTY 1
#define DEF_INS_PENALTY 1

static struct align_params_t *g_a = (struct align_params_t *)NULL;

/*******************************************************************/
lookup_levenstein(word, phrases, winner, conf, alignment, alen)
char *word;
MFS *phrases;
int *winner, **alignment, *alen;
float *conf;
{
   int i, distance, talen, mindist, min_i;
   struct align_params_t *a;
   int *talign, maxalen;

   init_align_params(&a, 1, DEF_SUB_PENALTY, DEF_DEL_PENALTY, DEF_INS_PENALTY);
   maxalen = strlen(word)+MAXMFSLENGTH;
   malloc_int(&talign, maxalen, "lookup_lvenstein : talign");
   malloc_int(alignment, maxalen, "lookup_lvenstein : alignment");

   mindist = ldv( word, phrases->values[0], a, NULL, &talen, talign );
   min_i = 0;
   memcpy((*alignment), talign, talen * sizeof(int));
   (*alen) = talen;
   for(i = 1; i < phrases->num; i++){
      distance = ldv( word, phrases->values[i], a, NULL, &talen, talign );
      if(distance < mindist){
         mindist = distance;
         min_i = i;
         memcpy((*alignment), talign, talen * sizeof(int));
         (*alen) = talen;
      }
   }
   free(talign);
   *winner = min_i;
}

/********************************************************************/
init_align_params(a, case_flag, sub_penalty, del_penalty, ins_penalty)
struct align_params_t **a;
int case_flag, sub_penalty, del_penalty, ins_penalty;
{
   int i, j, *dptr, *iptr, *sptr;
   static int ocase = -1, odel = -1, oins = -1, osub = -1;
   static int first = 1, d1len, d2len;

   if(first){
      d1len = NALIGNCHARS;
      d2len = NALIGNCHARS * NALIGNCHARS;
      first = 0;
   }

   /* if first time, then allocate and init globals */
   if(g_a == NULL){
      g_a = (struct align_params_t *)malloc(sizeof(struct align_params_t));
      if(g_a == NULL)
         syserr("init_align_params", "malloc", "g_a");
      g_a->case_sensitive = case_flag;
      malloc_int(&(g_a->subv), d2len, "init_align_params : g_a->subv");
      dptr = g_a->delv;
      iptr = g_a->insv;
      sptr = g_a->subv;
      for ( i=0; i<d1len; i++ ) {
         *dptr++ = del_penalty;
         *iptr++ = ins_penalty;
         for ( j=0; j<d1len; j++ )
            *sptr++ = sub_penalty;
      }
   }
   /* have flags or penalties changed ? */
   else{
      if(ocase != case_flag){
         g_a->case_sensitive = case_flag;
         ocase = case_flag;
      }
      if(odel != del_penalty){
         dptr = g_a->delv;
         for ( i=0; i<d1len; i++ )
            *dptr++ = del_penalty;
         odel = del_penalty;
      }
      if(oins != ins_penalty){
         dptr = g_a->insv;
         for ( i=0; i<d1len; i++ )
            *dptr++ = ins_penalty;
         oins = ins_penalty;
      }
      if(osub != sub_penalty){
         dptr = g_a->subv;
         for ( i=0; i<d2len; i++ )
            *dptr++ = sub_penalty;
         osub = sub_penalty;
      }
   }

   *a = g_a;
}

/*******************************************************************/
end_lookup_levenstein()
{
   if(g_a != NULL){
     free(g_a->subv);
     free(g_a);
     g_a = (struct align_params_t *)NULL;
   }
}
