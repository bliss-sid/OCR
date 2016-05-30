/*
# proc: pnnsearch - given init "close" vectors "centers" locate the nearest
# proc:             neighbors in a tree and classify points using a truncated
# proc:             pnn metric.
# proc: leaf_search - look at the points held in a leaf, and determine whether
# proc:             they are close enough to be used for classification.
# proc: tree_search - determine whether all points contained in a subtree are
# proc:             already inelligible to be used by the classifier, and if
# proc:             not recursively seacrh the children, best search first, for
# proc:             suitable neighbors
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <values.h>
#include <kdtree.h>

static float delligible;
static float dacceptable;
static float dminimum;
static float slopy;
static float *closedists;
static int   *closeclass;
static int ninstack;

static float LN10 = 2.302585093;	/* natural log of 10	*/
static float lambda = LAMBDA;

void pnnsearch(tree, searchpoints, nsearchpoints, nclasses, centers,
               cutoffs, ncutoffs, sigma, hyps, cons)
TREE *tree; float *searchpoints, *centers, *cutoffs, sigma, *cons;
int nsearchpoints, nclasses, ncutoffs, *hyps;
{
int i, k, n, d, nknownpoints, emptyset, ncuts;
float *acts, twoss, fexpy, sumacts, t;
float *ptr, dbest, dthis;

   nknownpoints = tree->npoints;

   malloc_flt(&acts, nclasses, "space for activations");
   malloc_flt(&closedists, nknownpoints, "space for close distances");
   malloc_int(&closeclass, nknownpoints, "space for close classes");

   twoss = 2.0 * sigma * sigma;
   slopy = lambda * LN10 * twoss;
   fexpy = -1.0 / twoss;

   for (i = 0, d = tree->datadimension ; i < nsearchpoints ; i++, searchpoints += d)
   {
      for ( k = 0, dbest = MAXFLOAT, ptr = centers ; k < nclasses ; k++, ptr += d )
      {
         for ( n = 0, dthis = 0.0 ; n < tree->datadimension ; n++ )
            if ((t = (float)fabs((double)(ptr[n] - searchpoints[n]))) > dthis)
               dthis = t;

         if (dthis < dbest)
            dbest = dthis;
      }

      for ( emptyset = 1, ncuts = 0 ; emptyset && ncuts < ncutoffs ; ncuts++ )
      {
         ninstack = 0;
         dacceptable = dbest * cutoffs[ncuts];
         dminimum = delligible = MAXFLOAT;

         tree_search(tree, searchpoints);

         for ( k = 0 ; k < ninstack && closedists[k] > delligible ; k++ );
         emptyset = (k == ninstack);
      }

      for ( k = 0 ; k < nclasses ; k++ )
         acts[k] = 0.0;

      for ( k = 0 ; k < ninstack ; k++ )
         if (closedists[k] <= delligible)
             acts[closeclass[k]] += (float)exp((double)(fexpy * closedists[k]));

      for ( k = 0, sumacts = 0.0 ; k < nclasses ; k++ )
         if (acts[k] > 0.0)
            sumacts += acts[k];

      for ( k = 0 ; k < nclasses ; k++ )
         if (acts[k] > 0.0)
            acts[k] /= sumacts;

      hyps[i] = k = nn_maxpf(acts, nclasses);
      cons[i] = acts[k];
   }

   free(acts);
   free(closedists);
   free(closeclass);
}


void leaf_search(tree, x)
TREE *tree; float *x;
{
int i, j;
float t, d;
float *y;
POINT *p;

   /* in some cases the leaf will contain no points, because no training	*/
   /* examples fell on the range that it is responsible for. This is more	*/
   /* likely to occur when the tree has many children and only a few points to	*/
   /* be held by them. eg 12 points in 9 kids there's no guarantee that there	*/
   /* will be one in each							*/

   for ( i = 0, p = tree->points ; i < tree->npoints ; i++, p++ )
   {
      y = p->data;
      for ( j = 0, d = 0.0 ; d < delligible && j < tree->datadimension ; j++ )
         t = y[j] - x[j], d += t * t;

      if (d < delligible)
      {
         closeclass[ninstack] = p->class;
         closedists[ninstack] = d;
         ninstack++;
         if (d < dminimum)
            delligible = (dminimum = d) + slopy;
      }
   }
}


void tree_search(tree, x)
TREE *tree; float *x;
{
float xsplit;
int i, besti;

   if (istreechildless(tree))		/* if there are no children then this 	*/
   {					/* is a leaf holding vectors to search	*/
      leaf_search(tree, x);
      return;
   }

   /* identify the interval in which the point lies, just a one dimensional	*/
   /* search over a small number of dividing ordinates				*/
   xsplit = x[tree->splitindex];
   besti = tree_whichinterval(tree, xsplit);

   tree_search(tree->children[besti], x);

   /* search other interval's trees in order of closeness to the	*/
   /* best interval; the further we stray the less likely the point	*/
   /* is to be there. In practice this gives only a small gain over	*/
   /* going through the list from left to right in order if index.	*/
   /* Of course the best one should be searched first. The children	*/
   /* must be arranged in ascending order of split values, L->R		*/
   for ( i = 1 ; besti-i >= 0 || besti+i < tree->nchild ; i++ )
   {
      if (besti - i >= 0)		/* trees left of the best one	*/
      {
         if (xsplit - dacceptable < tree->children[besti-i]->vmax &&
             xsplit + dacceptable > tree->children[besti-1]->vmin)
               tree_search(tree->children[besti-i], x);
      }

      if (besti + i < tree->nchild)	/* trees right of the best one 	*/
      {
         if (xsplit - dacceptable < tree->children[besti+i]->vmax &&
             xsplit + dacceptable > tree->children[besti+1]->vmin)
               tree_search(tree->children[besti+i], x);
      }
   }
}
