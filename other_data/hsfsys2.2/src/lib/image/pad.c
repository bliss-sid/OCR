/*
# proc: image_pad - pads an image to a new byte-aligned width and height based
# proc:             on new width and height dimensions.
*/

/* LINTLIBRARY */

#include <stdio.h>
#include <memory.h>
#include <sys/types.h>
#include <sys/param.h>

/*
 * Stan Janet
 * 2/14/91
 *
 * image_pad()
 *
 * image_pad() pads an 8-bit image's width and height to
 *	"padw" and "padh" bits. On return, "image" may point
 *	to a larger buffer.
 * New pixels outside original image are set to "bg".
 * Returns 1 if a new image was obtained via malloc(3),
 *	or 0 if no padding was necessary. Caller should
 *	free(3) the original image if appropriate.
 *
 */

#define SUBR_STR	"image_pad"


int image_pad(image,w,h,padw,padh,bg)
u_char **image;
u_int w, h, padw, padh;
int bg;
{
u_char *p, *new_image;
u_int new_size;
int diff_w, diff_h;
extern char *malloc();

if (image == (u_char **) NULL)
	fatalerr(SUBR_STR,"null image pointer address",(char *)NULL);
if (*image == (u_char *) NULL)
	fatalerr(SUBR_STR,"null image pointer",(char *)NULL);

if (padw == 0)
	fatalerr(SUBR_STR,"pad width is zero",(char *)NULL);
if (padh == 0)
	fatalerr(SUBR_STR,"pad height is zero",(char *)NULL);

if ((w == 0) || (h == 0))		/* no image to pad */
	return 0;

if ((w == padw) && (h == padh))		/* no growth */
	return 0;

new_size = padw * padh;
new_image = (u_char *) malloc(new_size);
if (new_image == (u_char *) NULL)
	fatalerr(SUBR_STR,"malloc failed",(char *)NULL);

p = new_image;
diff_w = (int) (padw - w);
diff_h = (int) (padh - h);
while (h--) {
        memcpy((char *)p, (char *)*image, (int)w);
	*image += w;
	p += w;
	if (diff_w) {
		(void) memset((char *)p, bg, diff_w);
		p += diff_w;
	}
}

if (diff_h)
	(void) memset((char *)p, bg, (int)padw * diff_h);

*image = new_image;
return 1;
}
