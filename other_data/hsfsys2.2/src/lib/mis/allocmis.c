/*
# proc: allocmis - allocates and initializes an mis structure with specified
# proc:            attributes and length.
# proc: reallocmis - reallocates or allocates if necessary an mis structure
# proc:              with specified attributes and length.
*/

#include <stdio.h>
#include <ihead.h>
#include <mis.h>

MIS *allocmis(filename,depth,bpi,entw,enth,ent_alloc)
char *filename;	/* ALLOWED TO BE NULL */
int entw, enth, ent_alloc,bpi,depth;
{
  MIS *mis;

  if ((entw <= 0) || (enth <= 0) || (ent_alloc <= 0))
     fatalerr("allocmis",
        "non positive parameter supplied", "either entw enth ent_alloc");

  if((mis = (MIS *)malloc(sizeof(MIS))) == NULL)
     syserr("allocmis","malloc","mis");
  mis->misw = entw;
  mis->mish = enth * ent_alloc;
  mis->misd = depth;
  mis->entw = entw;
  mis->enth = enth;
  mis->ent_num = 0;
  mis->ent_alloc = ent_alloc;
  /* the header id is to be filled in by the application or will */
  /* be automatically assigned within a call to writemisfile() */
  mis->head = buildmishdr(filename,depth,bpi,mis->misw,mis->mish,
                          mis->entw,mis->enth,mis->ent_alloc,UNCOMP);
  calloc_uchar(&(mis->data), SizeFromDepth(mis->misw,mis->mish,depth),
               "allocmis : mis->data");
  return(mis);
}

MIS *reallocmis(mis, depth, bpi, entw, enth, newlen)
MIS *mis; int depth, bpi, entw, enth, newlen;
{
   if (mis == NULL)
      return allocmis(NULL, depth, bpi, entw, enth, newlen);

   if (depth != mis->misd || entw != mis->entw || enth != mis->enth)
      fatalerr("reallocmis", "depth width or height", "incompatible");

   mis->ent_alloc = newlen;
   mis->mish = mis->enth * newlen;

   realloc_uchar(&(mis->data), SizeFromDepth(mis->misw, mis->mish, depth),
                 "reallocmis : mis->data");

   return mis;
}
