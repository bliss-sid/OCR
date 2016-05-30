/*
# proc: writemisfile - writes the contents of an mis structure to the
# proc:                specified file in IHead format.
*/

#include <stdio.h>
#include <ihead.h>
#include <mis.h>

void writemisfile(file,mis)
char *file;
MIS *mis;
{
   IHEAD *newhdr;
   int newmish, depth, bpi, compcode;

   if(strlen(mis->head->id) == 0)
      strcpy(mis->head->id, file);
   /* write what has been filled rather than allocated */
   if(mis->ent_num != mis->ent_alloc){
      newmish = mis->ent_num * mis->enth;
      depth = get_depth(mis->head);
      bpi = get_density(mis->head);
      compcode = get_compression(mis->head);
      newhdr = buildmishdr(file,depth,bpi,mis->misw,newmish,
               mis->entw,mis->enth,mis->ent_num,compcode);
      writeihdrfile(file,newhdr,mis->data);
      free(newhdr);
   }
   else
      writeihdrfile(file,mis->head,mis->data);
}
