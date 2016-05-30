/*
# proc: buildmishdr - given a list of mis attributes, returns a new
# proc:               allocated and initialized mis structure.
*/

/************************************************************/
/* Allocates, initializes, and returns an ihead structure   */
/* containing Calera specs for a given image file.          */
/************************************************************/
#include <stdio.h>
#include <ihead.h>
#include <mis.h>

IHEAD *buildmishdr(misfile,depth,bpi,misw,mish,entw,enth,ent_alloc,compcode)
char *misfile;
int depth,bpi, misw, mish, entw, enth, ent_alloc,compcode;
{
   IHEAD *head;
   char *fptr;

   if (ent_alloc * enth != mish)
	fatalerr("buildmishdr",misfile,"MIS height != entry height*entry count");
   if (entw != misw)
	fatalerr("buildmishdr",misfile,"MIS width != entry width");

   /* allocate an ihead structure */
   head = (IHEAD *)malloc(sizeof(IHEAD));
   if (head == NULL)
      syserr("buildmishdr","malloc",NULL);

   /* Null each byte in entire header buffer */
   nullihdr(head);

   /* get just the root file name of the image file passed */
   if(misfile != NULL){
      strippath(misfile,&fptr);
      /* create a header id string */
      (void) strcpy(head->id,fptr);
   }
   else
      head->id[0] = '\0';

   set_created(head);
   set_width(head, misw);
   set_height(head, mish);
   set_depth(head, depth);
   set_density(head, bpi);
   set_compression(head, compcode);
   set_complen(head, COMPLEN);
   set_align(head, ALIGN);
   set_unitsize(head, UNITSIZE);
   set_sigbit(head, SIGBIT);
   set_byte_order(head, BYTE_ORDER);
   set_pix_offset(head, PIXOFF);
   set_whitepix(head, WHITEPIX);
   set_issigned(head, SGND);
   set_rm_cm(head, R_M);
   set_tb_bt(head, T2B);
   set_lr_rl(head, L2R);
   set_parent(head, PARENT);
   set_par_x(head, entw);
   set_par_y(head, enth);

/* return the initialized structure */
return head;
}
