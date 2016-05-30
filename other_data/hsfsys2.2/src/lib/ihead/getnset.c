/*
# proc: get_id - get the identity attribute from an IHead structure.
# proc:
# proc: set_id - set the identity attribute from an IHead structure.
# proc:
# proc: get_created - get the creation date attribute from an IHead structure.
# proc:
# proc: set_created - set the creation date attribute of an IHead structure.
# proc:
# proc: get_width - get the pixel width attribute from an IHead structure.
# proc:
# proc: set_width - set the pixel width attribute of an IHead structure.
# proc:
# proc: get_height - get the pixel height attribute from an IHead structure.
# proc:
# proc: set_height - set the pixel height attribute of an IHead structure.
# proc:
# proc: get_depth - get the pixel depth attribute from an IHead structure.
# proc:
# proc: set_depth - set the pixel depth attribute of an IHead structure.
# proc:
# proc: get_density - get the pixel density attribute from an IHead structure.
# proc:
# proc: set_density - set the pixel density attribute of an IHead structure.
# proc:
# proc: get_compression - get the compression code attribute from an
# proc:                   IHead structure.
# proc: set_compression - set the compression code  attribute of an
# proc:                   IHead structure.
# proc: get_complen - get the compressed data length attribute from
# proc:               an IHead structure.
# proc: set_complen - set the compressed data length attribute of
# proc:               an IHead structure.
# proc: get_align - get the scanline alignment attribute from an
# proc:             IHead structure.
# proc: set_align - set the scanline alignment attribute of an IHead structure.
# proc:
# proc: get_unitsize - get the memory unit size attribute from
# proc:                an IHead structure.
# proc: set_unitsize - set the memory unit size attribute of
# proc:                an IHead structure.
# proc: get_sigbit - get the bit order attribute from an IHead structure.
# proc:
# proc: set_sigbit - set the bit order attribute of an IHead structure.
# proc:
# proc: get_byte_order - get the byte order attribute from an IHead structure.
# proc:
# proc: set_byte_order - set the byte order attribute of an IHead structure.
# proc:
*/
/*
# proc: get_pix_offset - get the column pixel offset attribute from
# proc:                  an IHead structure.
# proc: set_pix_offset - set the column pixel offset attribute of
# proc:                  an IHead structure.
# proc: get_whitepix - get the white pixel attribute from an IHead structure.
# proc:
# proc: set_whitepix - set the white pixel attribute of an IHead structure.
# proc:
# proc: get_issigned - get the memory unit signed/unsigned attribute from
# proc:                an IHead structure.
# proc: set_issigned - set the memory unit signed/unsigned attribute of
# proc:                an IHead structure.
# proc: get_rm_cm - get the row/column major attribute from an IHead structure.
# proc:
# proc: set_rm_cm - set the row/column major attribute of an IHead structure.
# proc:
# proc: get_tb_bt - get the top-to-bottom/bottom-to-top attribute from
# proc:             an IHead structure.
# proc: set_tb_bt - set the top-to-bottom/bottom-to-top attribute of
# proc:             an IHead structure.
# proc: get_lr_rl - get the right-to-left/left-to-right attribute from
# proc:             an IHead structure.
# proc: set_lr_rl - set the right-to-left/left-to-right attribute of
# proc:             an IHead structure.
# proc: get_parent - get the parent image attribute from an IHead structure.
# proc:
# proc: set_parent - set the parent image attribute of an IHead structure.
# proc:
# proc: get_par_x - get the x-coordinate cut from parent attribute from
# proc:             an IHead structure.
# proc: set_par_x - set the x-coordinate cut from parent attribute of
# proc:             an IHead structure.
# proc: get_par_y - get the y-coordinate cut from parent attribute from
# proc:             an IHead structure.
# proc: set_par_y - set the y-coordinate cut from parent attribute of
# proc:             an IHead structure.
*/

#include <stdio.h>
#include <string.h>
#include <ihead.h>
#include <defs.h>

char *get_id(head)
IHEAD *head;
{
  if (head==NULL)
     fatalerr("get_id","pointer head is NULL",NULL);
  return(strdup(head->id));
}

set_id(i,path)
IHEAD *i;
char *path;
{
   if (i == (IHEAD *) NULL)
	fatalerr("set_id","null ihead pointer",(char *)NULL);
   if (path == (char *) NULL)
	fatalerr("set_id","null filename pointer",(char *)NULL);

   while ((path != (char *) NULL) && (strlen(path) >= sizeof((*i).id)))
	path = strchr(path+1,'/');

   memset(i->id,0,sizeof((*i).id));
   if (path != (char *) NULL)
	(void) strcpy(i->id,path);
}

char *get_created(head)
IHEAD *head;
{
  if (head==NULL)
     fatalerr("get_created","pointer head is NULL",NULL);
  return(strdup(head->created));
}

set_created(head)
IHEAD *head;
{
  char *current_time();
  char *cur_time;

  if (head==NULL)
     fatalerr("set_created","pointer head is NULL",NULL);
  if ((cur_time = current_time()) == NULL)
     memset(head->created,0,DATELEN);
  else
    strcpy(head->created, cur_time);
}

get_width(head)
IHEAD *head;
{
  int width;

  if (head==NULL)
     fatalerr("get_width","pointer head is NULL",NULL);
  sscanf(head->width,"%d", &width);
  return(width);
}

set_width(head, width)
IHEAD *head;
int width;
{
  if (head==NULL)
     fatalerr("set_width","pointer head is NULL",NULL);
  memset(head->width,0,SHORT_CHARS);
  sprintf(head->width,"%d",width);
  return(True);
}

get_height(head)
IHEAD *head;
{
  int height;

  if (head==NULL)
     fatalerr("get_height","pointer head is NULL",NULL);
  sscanf(head->height,"%d", &height);
  return(height);
}

set_height(head, height)
IHEAD *head;
int height;
{
  if (head==NULL)
     fatalerr("set_height","pointer head is NULL",NULL);
  memset(head->height,0,SHORT_CHARS);
  sprintf(head->height,"%d",height);
  return(True);
}

int get_depth(head)
IHEAD *head;
{
  int depth;

  if (head==NULL)
     fatalerr("get_depth","pointer head is NULL",NULL);
  sscanf(head->depth,"%d", &depth);
  return(depth);
}

set_depth(head, depth)
IHEAD *head;
int depth;
{
  if (head==NULL)
     fatalerr("set_depth","pointer head is NULL",NULL);
  memset(head->depth,0,SHORT_CHARS);
  sprintf(head->depth,"%d",depth);
  return(True);
}

int get_density(head)
IHEAD *head;
{
  int density;

  if (head==NULL)
     fatalerr("get_density","pointer head is NULL",NULL);
  sscanf(head->density,"%d", &density);
  return(density);
}

set_density(head, density)
IHEAD *head;
int density;
{
  if (head==NULL)
     fatalerr("set_density","pointer head is NULL",NULL);
  memset(head->density,0,SHORT_CHARS);
  sprintf(head->density,"%d",density);
  return(True);
}

int get_compression(head)
IHEAD *head;
{
  int compression_code;

  if (head==NULL)
     fatalerr("get_compression","pointer head is NULL",NULL);
  sscanf(head->compress,"%d", &compression_code);
  return(compression_code);
}

set_compression(head, compression_code)
IHEAD *head;
int compression_code;
{
  if (head==NULL)
     fatalerr("set_compression","pointer head is NULL",NULL);
  if (valid_compression(compression_code)){
     memset(head->compress,0,SHORT_CHARS);
     sprintf(head->compress,"%d",compression_code);
  }
  else
     return(False);
  return(True);
}

int get_complen(head)
IHEAD *head;
{
  int complen;

  if (head==NULL)
     fatalerr("get_complen","pointer head is NULL",NULL);
  sscanf(head->complen,"%d", &complen);
  return(complen);
}

set_complen(head, complen)
IHEAD *head;
int complen;
{
  if (head==NULL)
     fatalerr("set_complen","pointer head is NULL",NULL);
  memset(head->complen, 0, SHORT_CHARS);
  sprintf(head->complen,"%d",complen);
  return(True);
}

int get_align(head)
IHEAD *head;
{
  int align;

  if (head==NULL)
     fatalerr("get_align","pointer head is NULL",NULL);
  sscanf(head->align,"%d", &align);
  return(align);
}

set_align(head, align)
IHEAD *head;
int align;
{
  if (head==NULL)
     fatalerr("set_align","pointer head is NULL",NULL);
  memset(head->align,0,SHORT_CHARS);
  sprintf(head->align,"%d",align);
  return(True);
}

int get_unitsize(head)
IHEAD *head;
{
  int unitsize;

  if (head==NULL)
     fatalerr("get_unitsize","pointer head is NULL",NULL);
  sscanf(head->unitsize,"%d", &unitsize);
  return(unitsize);
}

set_unitsize(head, unitsize)
IHEAD *head;
int unitsize;
{
  if (head==NULL)
     fatalerr("set_unitsize","pointer head is NULL",NULL);
  memset(head->unitsize,0,SHORT_CHARS);
  sprintf(head->unitsize,"%d",unitsize);
  return(True);
}

int get_sigbit(head)
IHEAD *head;
{
  int sigbit;

  if (head==NULL)
     fatalerr("get_sigbit","pointer head is NULL",NULL);
  sigbit = atoi(&(head->sigbit));
  return(sigbit);
}

set_sigbit(head, sigbit)
IHEAD *head;
int sigbit;
{
  if (head==NULL)
     fatalerr("set_sigbit","pointer head is NULL",NULL);
  
  if((sigbit == 0) || (sigbit == '0'))
     head->sigbit = '0';
  else if((sigbit == 1) || (sigbit == '1'))
     head->sigbit = '1';
  else
     fatalerr("set_sigbit","Sigbit must equal 1 (MSB Last) or 0 (MSB First).",NULL);

  return(True);
}

int get_byte_order(head)
IHEAD *head;
{
  int byte_order;

  if (head==NULL)
     fatalerr("get_byte_order","pointer head is NULL",NULL);
  byte_order = atoi(&(head->byte_order));
  return(byte_order);
}

set_byte_order(head, byte_order)
IHEAD *head;
int byte_order;
{
  if (head==NULL)
     fatalerr("set_byte_order","pointer head is NULL",NULL);
  head->byte_order = (char)byte_order;

  if((byte_order == 0) || (byte_order == '0'))
     head->byte_order = '0';
  else if((byte_order == 1) || (byte_order == '1'))
     head->byte_order = '1';
  else
     fatalerr("set_byte_order","Byte_order must equal 1 (Low-High) or 0 (High-Low).",NULL);

  return(True);
}

int get_pix_offset(head)
IHEAD *head;
{
  int pix_offset;

  if (head==NULL)
     fatalerr("get_pix_offset","pointer head is NULL",NULL);
  sscanf(head->pix_offset,"%d", &pix_offset);
  return(pix_offset);
}

set_pix_offset(head, pix_offset)
IHEAD *head;
int pix_offset;
{
  if (head==NULL)
     fatalerr("set_pix_offset","pointer head is NULL",NULL);
  memset(head->pix_offset,0,SHORT_CHARS);
  sprintf(head->pix_offset,"%d",pix_offset);
  return(True);
}

int get_whitepix(head)
IHEAD *head;
{
  int whitepix;

  if (head==NULL)
     fatalerr("get_whitepix","pointer head is NULL",NULL);
  sscanf(head->whitepix,"%d", &whitepix);
  return(whitepix);
}

set_whitepix(head, whitepix)
IHEAD *head;
int whitepix;
{
  if (head==NULL)
     fatalerr("set_whitepix","pointer head is NULL",NULL);
  memset(head->whitepix,0,SHORT_CHARS);
  sprintf(head->whitepix,"%d",whitepix);
  return(True);
}

int get_issigned(head)
IHEAD *head;
{
  int issigned;

  if (head==NULL)
     fatalerr("get_issigned","pointer head is NULL",NULL);
  issigned = atoi(&(head->issigned));
  return(issigned);
}

set_issigned(head, issigned)
IHEAD *head;
int issigned;
{
  if (head==NULL)
     fatalerr("set_issigned","pointer head is NULL",NULL);
  
  if((issigned == 0) || (issigned == '0'))
     head->issigned = '0';
  else if((issigned == 1) || (issigned == '1'))
     head->issigned = '1';
  else
     fatalerr("set_issigned","Issigned must equal 1 (signed) or 0 (unsigned).",NULL);

  return(True);
}

int get_rm_cm(head)
IHEAD *head;
{
  int rm_cm;

  if (head==NULL)
     fatalerr("get_rm_cm","pointer head is NULL",NULL);
  rm_cm = atoi(&(head->rm_cm));
  return(rm_cm);
}

set_rm_cm(head, rm_cm)
IHEAD *head;
int rm_cm;
{
  if (head==NULL)
     fatalerr("set_rm_cm","pointer head is NULL",NULL);
  
  if((rm_cm == 0) || (rm_cm == '0'))
     head->rm_cm = '0';
  else if((rm_cm == 1) || (rm_cm == '1'))
     head->rm_cm = '1';
  else
     fatalerr("set_rm_cm","Rm_cm must equal 1 (Column Major) or 0 (Row Major).",NULL);

  return(True);
}

int get_tb_bt(head)
IHEAD *head;
{
  int tb_bt;

  if (head==NULL)
     fatalerr("get_tb_bt","pointer head is NULL",NULL);
  tb_bt = atoi(&(head->tb_bt));
  return(tb_bt);
}

set_tb_bt(head, tb_bt)
IHEAD *head;
int tb_bt;
{
  if (head==NULL)
     fatalerr("set_tb_bt","pointer head is NULL",NULL);
  
  if((tb_bt == 0) || (tb_bt == '0'))
     head->tb_bt = '0';
  else if((tb_bt == 1) || (tb_bt == '1'))
     head->tb_bt = '1';
  else
     fatalerr("set_tb_bt","Tb_bt must equal 1 (bottom2top) or 0 (top2bottom).",NULL);

  return(True);
}

int get_lr_rl(head)
IHEAD *head;
{
  int lr_rl;

  if (head==NULL)
     fatalerr("get_lr_rl","pointer head is NULL",NULL);
  lr_rl = atoi(&(head->lr_rl));
  return(lr_rl);
}

set_lr_rl(head, lr_rl)
IHEAD *head;
int lr_rl;
{
  if (head==NULL)
     fatalerr("set_lr_rl","pointer head is NULL",NULL);
  
  if((lr_rl == 0) || (lr_rl == '0'))
     head->lr_rl = '0';
  else if((lr_rl == 1) || (lr_rl == '1'))
     head->lr_rl = '1';
  else
     fatalerr("set_lr_rl","Lr_rl must equal 1 (right2left) or 0 (left2right).",NULL);

  return(True);
}

char *get_parent(head)
IHEAD *head;
{
  if (head==NULL)
     fatalerr("get_parent","pointer head is NULL",NULL);
  return(strdup(head->parent));
}

/* LINTLIBRARY */
/*
 * Stan Janet
 * 12/14/90
 *
 * Clears all bytes of parent field of iheader,
 * then sets the field to the longest subpath
 * that will fit in the space provided in the structure,
 * if any.
 */

set_parent(i,path)
IHEAD *i;
char *path;
{
   if (i == (IHEAD *) NULL)
	fatalerr("set_parent","null ihead pointer",(char *)NULL);
   if (path == (char *) NULL)
	fatalerr("set_parent","null filename pointer",(char *)NULL);

   while ((path != (char *) NULL) && (strlen(path) >= sizeof((*i).parent)))
	path = strchr(path+1,'/');

   memset(i->parent,0,sizeof((*i).parent));
   if (path != (char *) NULL)
	(void) strcpy(i->parent,path);
}

int get_par_x(head)
IHEAD *head;
{
  int par_x;

  if (head==NULL)
     fatalerr("get_par_x","pointer head is NULL",NULL);
  sscanf(head->par_x,"%d", &par_x);
  return(par_x);
}

set_par_x(head, par_x)
IHEAD *head;
int par_x;
{
  if (head==NULL)
     fatalerr("set_par_x","pointer head is NULL",NULL);
  memset(head->par_x,0,SHORT_CHARS);
  sprintf(head->par_x,"%d",par_x);
  return(True);
}

int get_par_y(head)
IHEAD *head;
{
  int par_y;

  if (head==NULL)
     fatalerr("get_par_y","pointer head is NULL",NULL);
  sscanf(head->par_y,"%d", &par_y);
  return(par_y);
}

set_par_y(head, par_y)
IHEAD *head;
int par_y;
{
  if (head==NULL)
     fatalerr("set_par_y","pointer head is NULL",NULL);
  memset(head->par_y,0,SHORT_CHARS);
  sprintf(head->par_y,"%d",par_y);
  return(True);
}
