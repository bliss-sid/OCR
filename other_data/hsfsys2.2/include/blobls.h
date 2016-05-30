/************************************************************/
/*         Filename:     Blobls.h                           */
/*         Author:    Michael D. Garris                     */
/*         Date:      12/6/95                               */
/************************************************************/

typedef struct blobstruct{
   unsigned char *data;
   int x1, y1;                 /* parent x, y to blob's origin */
   int x2, y2;                 /* parent x, y to point opposite blob's origin */
   int cx, cy;                 /* parent x, y to blob's center point */
   int w, h, a, pixcnt;        /* width, height, area, and pixel count of blob */
} BLOB;

typedef struct bloblsstruct{
   int flags;
   int bpp;                    /* bits per pixel in blobs */
   int b_g;                    /* binary or grayscale flag for blobs */
   BLOB **blobs;               /* list of blob structures */
   int  num;                   /* number of blobs occupied */
   int  alloc;                 /* number of blobs allocated */
} BLOBLS;

#define BLOB_CHUNKS 100

#define BLOB_BIN    0
#define BLOB_GRAY   1

/* structure member flags */
#define BLOB_XY1S        0x1L
#define BLOB_XY2S        0x2L
#define BLOB_CXYS        0x4L
#define BLOB_WHS         0x8L
#define BLOB_AS          0x10L
#define BLOB_PIXCNTS     0x20L
#define BLOB_ALLS        0x3FL

#define blob_offsetof(_off_, _memb_) \
{  BLOB _b_; \
   _off_ = (int)((char *)&(_b_._memb_) - (char *)&(_b_)); \
}
