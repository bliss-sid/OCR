typedef struct apoint
{
  float *data;
  int class;
} POINT;

typedef struct arbre
{
  int  npoints;		/* number of points in this tree and the kids	*/
  int  allocpoints;	/* space allocated for the points' addresses	*/
  int  datadimension;	/* the dimensionality of the feature data	*/
  int  splitindex;	/* which feature dimension that is split upon	*/

  float *pointsdata;	/* contiguous memory of the floats in the tree	*/
  POINT *points;	/* struct containing class and ptr to a vector	*/

  int  nchild;
  float vmin;
  float vmax;
  struct arbre **children;	/* array of children pointers 		*/
} TREE;

#define istreechildless(tree) (tree->nchild == 0)

extern TREE *build_tree ();
extern TREE *alloc_tree ();
extern void init_tree ();
extern int tree_countleaves ();

extern void tree_findsplitvalues ();
extern void tree_entiles ();
extern float tree_onevariance ();
extern void tree_order ();
extern int comv ();

extern void tree_split ();
extern void addtotree ();

extern void pnnsearch ();
extern void tree_search ();
extern void leaf_search ();
extern int tree_whichinterval ();


extern void kdtreewrite ();
extern void kdtreeput ();
extern void kdleafput ();
extern void kdinfoput ();
extern TREE *kdtreeread ();
extern TREE *kdtreeget ();
extern int  kdinfoget ();


/* PNN parameters */
#define LAMBDA 4.0
