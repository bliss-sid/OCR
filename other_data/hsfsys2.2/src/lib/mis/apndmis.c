/*
# proc: appendmis - takes an image raster and appends it to the end of a
# proc:             homogeneous size and depth mis structure.
*/

#include <stdio.h>
#include <memory.h>
#include <ihead.h>
#include <mis.h>
#include <defs.h>

void appendmis(entmem,entw,enth,mis)
unsigned char *entmem;
int entw,enth;
MIS *mis;
{
   int misd, curh;
   int mislen, entlen;
   int incr, increased, bpi;

   if (mis->ent_num >= mis->ent_alloc)
   {
      bpi = get_density(mis->head);
      incr = mis->ent_num / 10;			/* the greater of 10% 	*/
      increased = mis->ent_num + max(10, incr);	/* and 10.		*/
      reallocmis(mis, mis->misd, bpi, mis->entw, mis->enth, increased);
   }

   if ((entw != mis->entw) || (enth != mis->enth))
      fatalerr("appendmis","noncompatible entry dimensions",NULL);
   misd = mis->misd;
   curh = mis->ent_num * mis->enth;
   mislen = SizeFromDepth(mis->misw, curh, misd);
   entlen = SizeFromDepth(entw, enth, misd);
   memcpy(mis->data+mislen, entmem, entlen);
   (mis->ent_num)++;
}
