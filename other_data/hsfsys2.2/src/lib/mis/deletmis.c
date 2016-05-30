/*
# proc: deletemis - removes an mis entry at the specified position and shifts
# proc:             all the following entries to fill the empty position.
*/

#include <stdio.h>
#include <memory.h>
#include <sys/types.h>
#include <ihead.h>
#include <mis.h>

void deletemis(mis,n)
MIS *mis;
int n;
{
   char index[BUFSIZ];
   int depth, entlen;
   int headlen, taillen;
   int endlen, emptylen;

   if ((n < 0) || (n >= mis->ent_num)){
        sprintf(index,"%d",n);
	fatalerr("deletemis","bad index",index);
   }

   if (mis->misw != mis->entw){
	sprintf(index,"mis width %d != image width %d", mis->misw, mis->entw);
	fatalerr("deletemis",index,(char *)NULL);
   }

   if (mis->mish != mis->ent_alloc * mis->enth)
	fatalerr("deletemis",
              "mis height != mis entries*image height",(char *)NULL);
   depth = get_depth(mis->head);
   mis->ent_num--;
   if (mis->ent_num < 1)
      fatalerr("deletemis","number of entries < 1",NULL);
   entlen = SizeFromDepth(mis->entw,mis->enth,depth);
   if (entlen < 1)
      fatalerr("deletemis","entry size < 1",NULL);
   headlen = n * entlen;
   taillen = (mis->ent_num - n) * entlen;
   endlen = mis->ent_num * entlen;
   emptylen = (mis->ent_alloc - mis->ent_num) * entlen;
   memcpy(mis->data+headlen,mis->data+headlen+entlen, taillen);
   memset(mis->data+endlen, 0, emptylen);
}

