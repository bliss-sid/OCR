/*
# proc: kdtreewrite - output a properly constructed tree to a named file
# proc:
# proc: kdtreeread - read the tree file, return the properly constructed tree
# proc:
# proc: kdtreeput - recursively write a tree and its subtrees to file
# proc:
# proc: kdinfoput - write a tree's node information to file
# proc:
# proc: kdleafput - write the pattern vectors stored in a tree's leaf
# proc:
# proc: kdtreeget - recursively read from file and attach subtrees to a tree
# proc:
# proc: kdinfoget - read a tree's node information from file
# proc:
*/

#include <stdio.h>
#include <kdtree.h>
#include <sex.h>

void kdtreewrite(tree, fn)
TREE *tree; char *fn;
{
FILE *fp;
   if ((fp = fopen(fn, "wb")) == NULL)
      syserr("kdtreewrite", "fopen", fn);
   kdtreeput(fp, tree);
   fclose(fp);
}


void kdtreeput(fp, tree)
FILE *fp; TREE *tree;
{
int isleaf = 0;	/* flag indicating not-a-leaf	*/
int i;

   if (istreechildless(tree))
   {
      kdleafput(fp, tree);
      return;
   }
   kdinfoput(fp, tree, isleaf);
   for ( i = 0 ; i < tree->nchild ; i++ )
      kdtreeput(fp, tree->children[i]);
}


void kdinfoput(fp, tree, isleaf)
FILE *fp; TREE *tree; int isleaf;
{
   unsigned int uint, *uptr;

   uint = swap_word(isleaf);
   if (1 != fwrite(&uint, sizeof(int  ), 1, fp))
      syserr("kdinfoput", "fwrite", "failed on npoints");

   uint = swap_word(tree->npoints);
   if (1 != fwrite(&uint, sizeof(int  ), 1, fp))
      syserr("kdinfoput", "fwrite", "failed on npoints");

   uint = swap_word(tree->datadimension);
   if (1 != fwrite(&uint, sizeof(int  ), 1, fp))
      syserr("kdinfoput", "fwrite", "failed on dimension");

   uint = swap_word(tree->nchild);
   if (1 != fwrite(&uint,        sizeof(int  ), 1, fp))
      syserr("kdinfoput", "fwrite", "failed on nchild");

   uint = swap_word(tree->splitindex);
   if (1 != fwrite(&uint, sizeof(int  ), 1, fp))
      syserr("kdinfoput", "fwrite", "failed on splitindex");

   uptr = (unsigned int *)&(tree->vmin);
   *uptr = swap_word(*uptr);
   if (1 != fwrite(uptr, sizeof(int  ), 1, fp))
      syserr("kdinfoput", "fwrite", "failed on vmin");
   *uptr = swap_word(*uptr);

   uptr = (unsigned int *)&(tree->vmax);
   *uptr = swap_word(*uptr);
   if (1 != fwrite(uptr, sizeof(int  ), 1, fp))
      syserr("kdinfoput", "fwrite", "failed on vmax");
   *uptr = swap_word(*uptr);
}


void kdleafput(fp, tree)
FILE *fp; TREE *tree;
{
int isleaf = 1;	/* flag indicating this --is-- a leaf node */
int i, n;
unsigned int *ptr2;

   kdinfoput(fp, tree, isleaf);

   n = tree->datadimension * tree->npoints;

   for(i = 0, ptr2 = (unsigned int *)tree->pointsdata; i < n; i++, ptr2++)
      *ptr2 = swap_word(*ptr2);
   if (n != fwrite(tree->pointsdata,  sizeof(float), n, fp))
      syserr("kdleafput", "fwrite", "failed on pointsdata");

   for ( i = 0 ; i < tree->npoints ; i++ ){
      tree->points[i].class = swap_word(tree->points[i].class);
      if (1 != fwrite(&tree->points[i].class, sizeof(int  ), 1, fp))
         syserr("kdleafput", "fwrite", "failed on point[i].class");
   }
}


TREE *kdtreeread(fn)
char *fn;
{
FILE *fp;
TREE *tree;

   if ((fp = fopen(fn, "rb")) == NULL)
      syserr("kdtreeread", "fopen", fn);
   tree = kdtreeget(fp);
   fclose(fp);
   return tree;
}


TREE *kdtreeget(fp)
FILE *fp;
{
TREE *tree;
int isleaf;

   if ((tree = (TREE *)malloc(sizeof(TREE))) == NULL)
      syserr("kdtreeread", "malloc", "space for TREE");
   init_tree(tree, NULL, 0, 0);

   isleaf = kdinfoget(fp, tree);

   if (!isleaf)
   {
      int i;

      if ((tree->children = (TREE **)malloc(tree->nchild * sizeof(TREE *))) == NULL)
         syserr("kdtreeget", "malloc", "space for TREE pointers");

      for ( i = 0 ; i < tree->nchild ; i++ )
         tree->children[i] = kdtreeget(fp);
   }
   else
   {
      int i, n;
      float *dst;
      unsigned int *uptr;

      tree->allocpoints = tree->npoints;
      n = tree->datadimension * tree->npoints;
      if ((tree->points     = (POINT *)malloc(tree->npoints * sizeof(POINT))) == NULL)
         syserr("kdtreeget", "malloc", "space for POINTs");
      if ((tree->pointsdata = (float *)malloc(            n * sizeof(float))) == NULL)
         syserr("kdtreeget", "malloc", "space for pointsdata");
      if (n != fread(tree->pointsdata, sizeof(float), n, fp))
         syserr("kdtreeget", "fread", "pointsdata");
      uptr = (unsigned int *)tree->pointsdata;
      for(i = 0; i < n; i++)
         *uptr++ = swap_word(*uptr);

      n = tree->datadimension;
      for ( i = 0, dst = tree->pointsdata ; i < tree->npoints ; i++, dst += n )
      {
         if (1 != fread(&tree->points[i].class, sizeof(int), 1, fp))
            syserr("kdtreeget", "fread", "points[i].class");
         tree->points[i].class = swap_word(tree->points[i].class);
         tree->points[i].data = dst;
      }
   }
   return tree;
}


int kdinfoget(fp, tree)
FILE *fp; TREE *tree;
{
int leaf;
unsigned int *uptr;

   if (1 != fread(&leaf,		sizeof(int  ), 1, fp))
      syserr("kdinfoget", "fread", "leaf field");
   leaf = swap_word(leaf);
   if (1 != fread(&tree->npoints,	sizeof(int  ), 1, fp))
      syserr("kdinfoget", "fread", "npoints field");
   tree->npoints = swap_word(tree->npoints);
   if (1 != fread(&tree->datadimension, sizeof(int  ), 1, fp))
      syserr("kdinfoget", "fread", "npoints field");
   tree->datadimension = swap_word(tree->datadimension);
   if (1 != fread(&tree->nchild,	sizeof(int  ), 1, fp))
      syserr("kdinfoget", "fread", "nchild field");
   tree->nchild = swap_word(tree->nchild);
   if (1 != fread(&tree->splitindex,	sizeof(int  ), 1, fp))
      syserr("kdinfoget", "fread", "splitindex field");
   tree->splitindex = swap_word(tree->splitindex);
   if (1 != fread(&tree->vmin,		sizeof(float), 1, fp))
      syserr("kdinfoget", "fread", "vmin field");
   uptr = (unsigned int *)&(tree->vmin);
   *uptr = swap_word(*uptr);
   if (1 != fread(&tree->vmax,		sizeof(float), 1, fp))
      syserr("kdinfoget", "fread", "vmax field");
   uptr = (unsigned int *)&(tree->vmax);
   *uptr = swap_word(*uptr);

   return leaf;
}
