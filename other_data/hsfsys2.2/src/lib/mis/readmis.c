/*
# proc: readmisfile - reads the contents of the specified file into a new
# proc:               mis structure.
# proc: readmishdr - reads the IHead information from the specified file into a
# proc:              new mis structure.
*/

#include <stdio.h>
#include <ihead.h>
#include <mis.h>

/***************************************************************************/
MIS *readmisfile(file)
char *file;
{
   MIS *mis;
   IHEAD *head;
   unsigned char *buf;
   int misw, mish, bpi, entw, enth, ent_num;

   ReadBinaryRaster(file,&head,&buf,&bpi,&misw,&mish);
   if (sscanf(head->par_x,"%d",&entw) != 1)
      fatalerr("readmisfile",file,"cannot read entry width from par_x field");
   if (sscanf(head->par_y,"%d",&enth) != 1)
      fatalerr("readmisfile",file,"cannot read entry height from par_y field");
   if ((misw <= 0) || (mish <= 0))
      fatalerr("readmisfile",file,"bad MIS dimension(s)");
   if ((entw <= 0) || (enth <= 0))
      fatalerr("readmisfile",file,"bad entry dimension(s)");
   if (entw != misw)
      fatalerr("readmisfile",file,"MIS width != entry width");
   if (mish % enth != 0)
      fatalerr("readmisfile",file,"MIS height not a multiplt of entry height");
   ent_num = mish / enth;
   if((mis = (MIS *)malloc(sizeof(MIS))) == NULL)
      syserr("readmisfile","malloc","mis");
   mis->head = head;
   mis->data = buf;
   mis->misw = misw;
   mis->mish = mish;
   mis->misd = 1;
   mis->entw = entw;
   mis->enth = enth;
   mis->ent_num = ent_num;
   mis->ent_alloc = ent_num;
   return(mis);
}

/***************************************************************************/
MIS *readmishdr(file)
char *file;
{
IHEAD *readihdr();
IHEAD *head;
MIS   *mis;
FILE  *fp;
int   height, n;

   if ((fp = fopen(file, "rb")) == NULL)
      syserr("readmishdr",file,"fopen failed");

   if ((mis = (MIS *)malloc(sizeof(MIS))) == NULL)
      syserr("readmishdr","malloc","mis");

   head = readihdr(fp);
   fclose(fp);

   if ((n = sscanf(head->par_x,  "%d",&(mis->entw))) != 1)
      fatalerr("readmishdr",file,"cannot read entry width from par_x field");
   if ((n = sscanf(head->par_y,  "%d",&(mis->enth))) != 1)
      fatalerr("readmishdr",file,"cannot read entry height from par_y field");
   if ((n = sscanf(head->depth,  "%d",&(mis->misd))) != 1)
      fatalerr("readmishdr",file,"sscanf failed on depth field");
   if ((n = sscanf(head->width,  "%d",&(mis->misw))) != 1)
      fatalerr("readmishdr",file,"sscanf failed on width field");
   if ((n = sscanf(head->height, "%d",&height     )) != 1)
      fatalerr("readmishdr",file,"sscanf failed on height field");

   mis->ent_num   = height / mis->enth;
   mis->ent_alloc = height / mis->enth;
   mis->mish      = height;
   mis->head      = head;
   mis->data      = NULL;

   if ((mis->misw <= 0) || (mis->mish <= 0))
     fatalerr("readmisfile",file,"bad MIS dimension(s)");
   if ((mis->entw <= 0) || (mis->enth <= 0))
     fatalerr("readmisfile",file,"bad entry dimension(s)");
   if (mis->entw != mis->misw)
     fatalerr("readmisfile",file,"MIS width != entry width");
   if (mis->mish % mis->enth != 0)
     fatalerr("readmisfile",file,"MIS height not a multiple of entry height");

   return(mis);
}

