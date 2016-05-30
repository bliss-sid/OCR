/*
# proc: get_runs8 - extracts and returns a list of all the horizontal runs from
# proc:             the binary charimage passed.
# proc: next_run - resumes search and extracts the next horizontal run in the
# proc:            binary charimage passed.
# proc: add_run - adds the start point and the length of a run to the list
# proc:           of runs.
# proc: realloc_runs - allocates (and reallocates if necessary) the list of
# proc:                runs as the list becomes full.
*/

#include <stdio.h>
#include <defs.h>

#define RUN_CHUNKS     1000    /* reallocate run lists in incremental chunks */

/**********************************************************/
get_runs8(rx, ry, rlen, nruns, ralloc, cdata, w, h)
int **rx, **ry, **rlen, *nruns, *ralloc;
unsigned char *cdata;
int w, h;
{
   int cx, cy;
   int trx, try, trlen;

   *ralloc = 0;
   *nruns = 0;
   realloc_runs(rx, ry, rlen, ralloc, RUN_CHUNKS);

   cx = 0;
   cy = 0;
   while(next_run(&trx, &try, &trlen, cdata, &cx, &cy, w, h)){
      add_run(trx, try, trlen, rx, ry, rlen, nruns, ralloc);
   }
}

/**********************************************************/
next_run(trx, try, trlen, cdata, cx, cy, w, h)
int *trx, *try, *trlen, *cx, *cy, w, h;
unsigned char *cdata;
{
   unsigned char *cptr, *end, *endline;

   /* set pointer to resume search */
   cptr = cdata + ((*cy) * w) + (*cx);
   /* compute address for end of image */
   end = cdata + (w*h);
   /* compute address for end of current line */
   /* (actually beginning of next line ) */
   endline = cdata + (((*cy)+1) * w);

   /* while not at the end of the image ... */
   while(cptr < end){
      /* while not at the end of the line and have white pixels ... */
      while((cptr < endline) && (*cptr == 0)){
         cptr++;
         (*cx)++;
      }
      /* if at the end of the line (at beginning of next line ) */
      if(cptr == endline){
         /* reset x index and bump y index */
         (*cx) = 0;
         (*cy)++;
         /* bump end of line address */
         endline += w;
      }
      /* otherwise not at the end of the line, */
      /* so must be at the start of a new run of black pixels  ... */
      else{
         /* store the start position */
         *trx = (*cx);
         *try = (*cy);
         *trlen = 0;
         /* count the length of the run ... */
         while((cptr < endline) && (*cptr == 1)){
            cptr++;
            (*cx)++;
            (*trlen)++;
         }
         /* we now have the run, so before returning */
         /* if at the end of the line (at beginning of next line ) ... */
         if(cptr == endline){
            /* reset x index and bump y index */
            (*cx) = 0;
            (*cy)++;
         }
         return(TRUE);
      }
   }
   /* otherwise, at the end of the image and no more runs found */
   return(FALSE);
}

/**********************************************************/
add_run(trx, try, trlen, rx, ry, rlen, nruns, ralloc)
int trx, try, trlen, **rx, **ry, **rlen, *nruns, *ralloc;
{
   if(*nruns >= *ralloc)
      realloc_runs(rx, ry, rlen, ralloc, RUN_CHUNKS);
   (*rx)[*nruns] = trx;
   (*ry)[*nruns] = try;
   (*rlen)[*nruns] = trlen;
   (*nruns)++;
}

/**********************************************************/
realloc_runs(rx, ry, rlen, ralloc, incr)
int **rx, **ry, **rlen, *ralloc, incr;
{
   /* allocate for the first time ... */
   if((*ralloc) == 0){
      (*ralloc) += incr;
      malloc_int(rx, *ralloc, "realloc_runs : rx");
      malloc_int(ry, *ralloc, "realloc_runs : ry");
      malloc_int(rlen, *ralloc, "realloc_runs : rlen");
      return;
   }

   /* reallocate if previously allocated lists are full ... */
   (*ralloc) += incr;
   realloc_int(rx, *ralloc, "realloc_runs : rx");
   realloc_int(ry, *ralloc, "realloc_runs : ry");
   realloc_int(rlen, *ralloc, "realloc_runs : rlen");
}
