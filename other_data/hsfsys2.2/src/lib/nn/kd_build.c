/*
# proc: build_tree - create a tree and insert a specified number of floating
# proc:              point vectors of specified class and dimensionality
# proc:              into a tree, and return it.
# proc: tree_split - recursivly distribute the points in a tree into subtrees
# proc:              and attach them as children to the tree
# proc: tree_findsplitvalues - indentify a component to control the division
# proc:              of a tree, and call an assignment subroutine
# proc: tree_entiles - assign suitable ranges for the coefficient on
# proc:              which the tree's points are subdivided
# proc: tree_onevariance - compute the variance of a specified component of a
# proc:              tree's point vectors.
# proc: addtotree - add one point to a tree
# proc:
# proc: init_tree - initialize the elements of a tree
# proc:
# proc: tree_countleaves - count the leaves contained by a tree
# proc:
# proc: tree_whichinterval - count the leaves contained by a tree
# proc:
# proc: tree_order - reorder floating vectors pointed to by a tree into
# proc:		     a contiguous memory block, to avoid cache misses.
# proc: tree_free - release memory allocated and hel by a tree
# proc:
# proc: istreechildless - macro (used to be a function) that indicates whether
# proc:          tree is a node (which has children) or a leaf (which does not).
# proc:          a tree has children or points but not both.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <values.h>
#include <kdtree.h>

static int nchild = 3;
static minpointspernode = 14;

TREE *build_tree(points, class, npoints, ni)
float *points; int *class, npoints, ni;
{
int i;
TREE *tree;
POINT *pts;

   if ((tree = (TREE *)malloc(sizeof(TREE))) == NULL)
      syserr("build_tree", "malloc", "space for a TREE");
   if ((pts = (POINT *)malloc(npoints * sizeof(POINT))) == NULL)
      syserr("build_tree", "malloc", "space for points TREE");

   init_tree(tree, (float *)points, npoints, ni);
   for ( i = 0 ; i < npoints ; i++, points += ni )
      pts[i].data = points,		/* just an address somewhere	*/
      pts[i].class = class[i];		/* integer class of the point	*/

   for ( i = 0 ; i < npoints ; i++ )
      addtotree(tree, &pts[i]);

   free(pts);
   tree_split(tree, minpointspernode, nchild);
   tree_order(tree);
   return tree;
}


void tree_split(tree, minpointspernode, nchild)
TREE *tree; int minpointspernode, nchild;
{
int i, oldc, allinsameinterval, c, d;

   if (tree->npoints <= minpointspernode)
      return;

   tree->nchild = nchild;
   if ((tree->children = (TREE **)malloc(nchild * sizeof(TREE *))) == NULL)
      syserr("tree_split", "malloc", "space for child pointers");

   for ( c = 0 ; c < nchild ; c++ )
   {
      if ((tree->children[c] = (TREE *)malloc(sizeof(TREE))) == NULL)
         syserr("tree_split", "malloc", "space for a child");
      init_tree(tree->children[c],
         tree->pointsdata, minpointspernode, tree->datadimension);
   }

   tree_findsplitvalues(tree);

   /* find which child this point should be passed down to, ie. look	*/
   /* for the interval in which the value lies				*/
   for ( i = 0, d = tree->splitindex ; i < tree->npoints ; i++ )
   {
      c = tree_whichinterval(tree, tree->points[i].data[d]);
      allinsameinterval = ( i == 0 || (allinsameinterval && oldc == c) );
      oldc = c;
   }

   if (allinsameinterval)
   {
      for ( c = 0 ; c < nchild ; c++ )
         free(tree->children[c]);
      free(tree->children);
      tree->children = NULL;
      tree->nchild = 0;
   }
   else
   {
      for ( i = 0, d = tree->splitindex ; i < tree->npoints ; i++ )
      {
         c = tree_whichinterval(tree, tree->points[i].data[d]);
         addtotree(tree->children[c], &tree->points[i]);
      }

      for ( c = 0 ; c < nchild ; c++ )
         tree_split(tree->children[c], minpointspernode, nchild);

      tree->allocpoints = 0; /* indicate that there is no allocated space, do not	*/
      free(tree->points);	  /* set npoints=0 since that is the number of points	*/
      tree->points = NULL;	  /* contained here PLUS in the children		*/
   }
}


void tree_findsplitvalues(tree)
TREE *tree;
{
int d;
float vari, maxvari = 0.0;

   for ( d = 0 ; d < tree->datadimension ; d++ )
      if (maxvari < (vari = tree_onevariance(tree, d)))
      {
         maxvari = vari;
         tree->splitindex = d;
      }

   tree_entiles(tree, tree->splitindex);
}


float tree_onevariance(tree, d)
TREE *tree; int d;
{
int i;
float vari = 0.0, mean = 0.0;
float *point;

   for ( i = 0 ; i < tree->npoints; i++ )
   {
      point = tree->points[i].data;
      mean += point[d];
      vari += point[d] * point[d];
   }

   mean /= (float)tree->npoints;
   vari /= (float)tree->npoints;
   vari -= mean * mean;
   return vari;
}

int comv(a, b)
float *a, *b;
{
float x = *a, y = *b;
   return (x < y) ? -1 : ((x > y) ? 1 : 0);
}

void tree_entiles(tree, d)
TREE *tree; int d;
{
int i, bin;
float *vals;

   bin = tree->npoints / tree->nchild;
   malloc_flt(&vals, tree->npoints, "tree_entiles");

   for ( i = 0 ; i < tree->npoints ; i++ )
      vals[i] = tree->points[i].data[d];

   qsort((void *)vals, (size_t)tree->npoints, sizeof(float), comv);

   tree->children[0]->vmin = -MAXFLOAT;
   for ( i = 1 ; i < tree->nchild ; i++ )
      tree->children[i]->vmin = tree->children[i-1]->vmax = vals[i*bin];
   tree->children[i-1]->vmax = MAXFLOAT;

   free(vals);
}



void addtotree(tree, point)
TREE *tree; POINT *point;
{
   if (tree->npoints >= tree->allocpoints)
   {
      tree->allocpoints += 30;
      tree->points = (tree->points == NULL) ?
          (POINT *)malloc(tree->allocpoints * sizeof(POINT)) :
          (POINT *)realloc(tree->points, tree->allocpoints * sizeof(POINT));

      if (tree->points == NULL)
         syserr("addtotree", "realloc", "space for 30 more POINTs");
   }
   tree->points[tree->npoints] = *point;
   tree->npoints++;
}

void init_tree(tree, points, npoints, nidim)
TREE *tree; int npoints, nidim; float *points;
{
    tree->datadimension = nidim;
    tree->splitindex = -1;
    tree->vmin = MAXFLOAT; tree->vmax = -MAXFLOAT; /* sic! wrong way round */

    tree->pointsdata = points;
    tree->points = NULL;
    tree->npoints = 0;
    tree->allocpoints = 0;

    tree->nchild = 0;
    tree->children = NULL;
}

int tree_countleaves(tree)
TREE *tree;
{
int i, count = 0;

   if (istreechildless(tree))
      return 1;

   for ( i = 0 ; i < tree->nchild ; i++ )
      count += tree_countleaves(tree->children[i]);

   return count;
}

/* actually get under the hood and copy the pointed to floating vectors */
/* (not just the pointers to them) to the local leaf, this allows 	*/
/* contiguous access by the machine, keeping things in the cache and	*/
/* avoiding paging, which is useful for CPU's to get speed. icky but...	*/
void tree_order(tree)
TREE *tree;
{
int i;

   if (istreechildless(tree))
   {
      if (tree->npoints > 0)
      {
         float *dat, *dst;
         POINT *pts;

         if ((pts = (POINT *)malloc(tree->npoints * sizeof(POINT))) == NULL)
            syserr("tree_order", "malloc", "space for POINTs");
         malloc_flt(&dat, tree->datadimension * tree->npoints, "tree_order");

         for ( i = 0, dst = dat ; i < tree->npoints ; i++, dst += tree->datadimension )
         {
            pts[i].class = tree->points[i].class;
            memcpy(dst, tree->points[i].data, tree->datadimension * sizeof(float));
            pts[i].data  = dst;
         }

         free(tree->points);
         tree->pointsdata = dat;
         tree->points = pts;
         tree->allocpoints = tree->npoints;
      }
   }
   else
      for ( i = 0 ; i < tree->nchild ; i++ )
         tree_order(tree->children[i]);
}


/* find which interval the number value lies in, the index found	*/
/* indicates the child tree which should contain such a point 		*/
int tree_whichinterval(t, val)
TREE *t; float val;
{
int i;
   for ( i = 0 ; i < t->nchild ; i++ )
      if ( val >= t->children[i]->vmin && val <= t->children[i]->vmax )
         return i;

   fatalerr("tree_whichinterval", "value", "not in any interval");
}


void treefree(tree)
TREE *tree;
{
int i;

   if (istreechildless(tree))
   {
      if (tree->pointsdata != NULL) free(tree->pointsdata);
      if (tree->points != NULL) free(tree->points);
   }
   else
   {
      for ( i = 0 ; i < tree->nchild ; i++ )
         treefree(tree->children[i]);
      free(tree->children);
   }
   free(tree);
}
