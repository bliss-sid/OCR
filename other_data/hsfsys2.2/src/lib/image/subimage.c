/*
# proc: subimage_copy - copies an 8-bit subimage to another image at a
# proc:                 specified location.
# proc: subimage_and - uses a logical AND operator to copy an 8-bit subimage
# proc:                to another image at a specified location.
# proc: subimage_or - uses a logical OR operator to copy an 8-bit subimage
# proc:               to another image at a specified location.
# proc: subimage_xor - uses a logical XOR operator to copy an 8-bit subimage
# proc:                to another image at a specified location.
# proc: subimage_add - copies an 8-bit subimage to another image adding the
# proc:                subimage pixels to the pixels at the specified location.
# proc: subimage_sub - copies an 8-bit subimage to another image subtracting
# proc:                subimage pixels from pixels at the specified location.
# proc: subimage_lookup - addresses a lookup table with the pixels from an
# proc:                   8-bit subimage and assignes the lookups to the
# proc:                   destination image at the specified location.
# proc: subimage_addcon - adds a constant to the pixels in an 8-bit subimage.
# proc:
# proc: subimage_subcon - subtracts a constant from the pixels in an 8-bit
# proc:                   subimage.
# proc: subimage_multcon - multiplies a constant to the pixels in an 8-bit
# proc:                    subimage.
# proc: subimage_divcon - divides the pixels in an 8-bit subimage by a
# proc:                   constant.
# proc: subimage_fmultcon - multiplies a floating point constant to the pixels
# proc:                     in an 8-bit subimage.
# proc: subimage_fdivcon - divides the pixels in an 8-bit subimage by a
# proc:                    floating point constant.
# proc: subimage_set - sets all the pixels in an 8-bit subimage to a specified
# proc:                value.
# proc: subimage_zeroes - sets all the pixels in an 8-bit subimage to zero.
# proc:
# proc: subimage_ones - sets all the pixels in an 8-bit subimage to one.
# proc:
# proc: subimage_invert - inverts all the pixels in an 8-bit subimage.
# proc:
*/

/* LINTLIBRARY */

#include <stdio.h>
#include <values.h>
#include <sys/types.h>
#include <sys/param.h>


#define LUTSIZE		(1 << BITSPERBYTE)
#define MAX_UCHAR	(LUTSIZE - 1)


/***********************************************************************/

subimage_copy (		src, srcw, srch,
			dst, dstw, dsth,
			srcx, srcy, w, h, dstx, dsty )
register u_char *src, *dst;
u_int srcw, srch, dstw, dsth, w, h;
int srcx, srcy, dstx, dsty;
{
register u_int i;
int dstdx, srcdx;

src += srcy * srcw + srcx;
srcdx = srcw - w;

dst += dsty * dstw + dstx;
dstdx = dstw - w;

while (h--) {
	i = w;
	while (i--)
		*dst++ = *src++;
	dst += dstdx;
	src += srcdx;
}
}

/***********************************************************************/

subimage_and (		src, srcw, srch,
			dst, dstw, dsth,
			srcx, srcy, w, h, dstx, dsty )
register u_char *src, *dst;
u_int srcw, srch, dstw, dsth, w, h;
int srcx, srcy, dstx, dsty;
{
register u_int i;
int dstdx, srcdx;

src += srcy * srcw + srcx;
srcdx = srcw - w;

dst += dsty * dstw + dstx;
dstdx = dstw - w;

while (h--) {
	i = w;
	while (i--)
		*dst++ &= *src++;
	dst += dstdx;
	src += srcdx;
}
}

/***********************************************************************/

subimage_or (		src, srcw, srch,
			dst, dstw, dsth,
			srcx, srcy, w, h, dstx, dsty )
register u_char *src, *dst;
u_int srcw, srch, dstw, dsth, w, h;
int srcx, srcy, dstx, dsty;
{
register u_int i;
int dstdx, srcdx;

src += srcy * srcw + srcx;
srcdx = srcw - w;

dst += dsty * dstw + dstx;
dstdx = dstw - w;

while (h--) {
	i = w;
	while (i--)
		*dst++ |= *src++;
	dst += dstdx;
	src += srcdx;
}
}

/***********************************************************************/

subimage_xor (		src, srcw, srch,
			dst, dstw, dsth,
			srcx, srcy, w, h, dstx, dsty )
register u_char *src, *dst;
u_int srcw, srch, dstw, dsth, w, h;
int srcx, srcy, dstx, dsty;
{
register u_int i;
int dstdx, srcdx;

src += srcy * srcw + srcx;
srcdx = srcw - w;

dst += dsty * dstw + dstx;
dstdx = dstw - w;

while (h--) {
	i = w;
	while (i--)
		*dst++ ^= *src++;
	dst += dstdx;
	src += srcdx;
}
}

/***********************************************************************/

subimage_add (		src, srcw, srch,
			dst, dstw, dsth,
			srcx, srcy, w, h, dstx, dsty )
register u_char *src, *dst;
u_int srcw, srch, dstw, dsth, w, h;
int srcx, srcy, dstx, dsty;
{
register u_int i, sum;
int dstdx, srcdx;

src += srcy * srcw + srcx;
srcdx = srcw - w;

dst += dsty * dstw + dstx;
dstdx = dstw - w;

while (h--) {
	i = w;
	while (i--) {
		sum = (u_int) *dst + (u_int) *src++;
		if (sum >= MAX_UCHAR)
			*dst++ = MAX_UCHAR;
		else
			*dst++ = sum;
	}
	dst += dstdx;
	src += srcdx;
}
}

/***********************************************************************/

subimage_sub (		src, srcw, srch,
			dst, dstw, dsth,
			srcx, srcy, w, h, dstx, dsty )
register u_char *src, *dst;
u_int srcw, srch, dstw, dsth, w, h;
int srcx, srcy, dstx, dsty;
{
register u_int i;
int dstdx, srcdx;

src += srcy * srcw + srcx;
srcdx = srcw - w;

dst += dsty * dstw + dstx;
dstdx = dstw - w;

while (h--) {
	i = w;
	while (i--) {
		if (*src >= *dst) {
			*dst++ = 0;
			src++;
		} else
			*dst++ -= *src++;
	}
	dst += dstdx;
	src += srcdx;
}
}

/***********************************************************************/

subimage_lookup ( lut, dst, dstw, dsth, dstx, dsty, w, h )
register u_char *lut, *dst;
u_int dstw, dsth, w, h;
int dstx, dsty;
{
register u_int i;
int dstdx;

dst += dsty * dstw + dstx;
dstdx = dstw - w;

while (h--) {
	i = w;
	while (i--) {
		*dst = lut[*dst];
		dst++;
	}
	dst += dstdx;
}
}

/***********************************************************************/

subimage_addcon ( n, dst, dstw, dsth, dstx, dsty, w, h )
register int n;
register u_char *dst;
u_int dstw, dsth, w, h;
int dstx, dsty;
{
u_char lut[LUTSIZE];
register u_int i, sum;

if (n < 0)
	subimage_subcon(-n,dst,dstw,dsth,dstx,dsty,w,h);
else {
	for (i=0; i<LUTSIZE; i++) {
		sum = i + n;
		if (sum >= MAX_UCHAR)
			lut[i] = MAX_UCHAR;
		else
			lut[i] = sum;
	}
	subimage_lookup(lut,dst,dstw,dsth,dstx,dsty,w,h);
}
}

/***********************************************************************/

subimage_subcon ( n, dst, dstw, dsth, dstx, dsty, w, h )
register int n;
register u_char *dst;
u_int dstw, dsth, w, h;
int dstx, dsty;
{
u_char lut[LUTSIZE];
register u_int i;

if (n < 0)
	subimage_addcon(-n,dst,dstw,dsth,dstx,dsty,w,h);
else {
	for (i=0; i<LUTSIZE; i++) {
		if ((u_int) n >= i)
			lut[i] = 0;
		else
			lut[i] = i - (u_int) n;
	}
	subimage_lookup(lut,dst,dstw,dsth,dstx,dsty,w,h);
}
}

/***********************************************************************/

subimage_multcon ( n, dst, dstw, dsth, dstx, dsty, w, h )
register int n;
register u_char *dst;
u_int dstw, dsth, w, h;
int dstx, dsty;
{
u_char lut[LUTSIZE];
register u_int i, product;

if (n < 0)
	fatalerr("subimage_multcon",
			"multiply by negative number",
			(char *)NULL);
for (i=0; i<LUTSIZE; i++) {
	product = i * n;
	if (product >= MAX_UCHAR)
		lut[i] = MAX_UCHAR;
	else
		lut[i] = product;
}
subimage_lookup(lut,dst,dstw,dsth,dstx,dsty,w,h);
}

/***********************************************************************/

subimage_divcon ( n, dst, dstw, dsth, dstx, dsty, w, h )
register int n;
register u_char *dst;
u_int dstw, dsth, w, h;
int dstx, dsty;
{
u_char lut[LUTSIZE];
register u_int i;

if (n < 0)
	fatalerr("subimage_divcon",
			"divide by negative number",
			(char *)NULL);
if (n == 0)
	fatalerr("subimage_divcon",
			"divide by zero",
			(char *)NULL);
for (i=0; i<LUTSIZE; i++)
	lut[i] = i / (u_int) n;
subimage_lookup(lut,dst,dstw,dsth,dstx,dsty,w,h);
}

/***********************************************************************/

subimage_fmultcon ( f, dst, dstw, dsth, dstx, dsty, w, h )
register double f;
register u_char *dst;
u_int dstw, dsth, w, h;
int dstx, dsty;
{
u_char lut[LUTSIZE];
register u_int i, product;

if (f < 0.0)
	fatalerr("subimage_fmultcon",
			"multiply by negative number",
			(char *)NULL);
for (i=0; i<LUTSIZE; i++) {
	product = (u_int) (i * f);
	if (product >= MAX_UCHAR)
		lut[i] = MAX_UCHAR;
	else
		lut[i] = product;
}
subimage_lookup(lut,dst,dstw,dsth,dstx,dsty,w,h);
}

/***********************************************************************/

subimage_fdivcon ( f, dst, dstw, dsth, dstx, dsty, w, h )
register int f;
register u_char *dst;
u_int dstw, dsth, w, h;
int dstx, dsty;
{
if (f < 0.0)
	fatalerr("subimage_fdivcon",
		"divide by negative number",
		(char *)NULL);
if (f == 0.0)
	fatalerr("subimage_fdivcon",
		"divide by zero",
		(char *)NULL);
subimage_fmultcon(1.0/f,dst,dstw,dsth,dstx,dsty,w,h);
}

/***********************************************************************/

subimage_set ( n, dst, dstw, dsth, dstx, dsty, w, h )
register int n;
register u_char *dst;
u_int dstw, dsth, w, h;
int dstx, dsty;
{
u_char lut[LUTSIZE];
register u_int i;

if (n < 0)
	fatalerr("subimage_set",
			"value out of range",
			(char *)NULL);
for (i=0; i<LUTSIZE; i++)
	lut[i] = (char) n;
subimage_lookup(lut,dst,dstw,dsth,dstx,dsty,w,h);
}

/***********************************************************************/

subimage_zeroes ( dst, dstw, dsth, dstx, dsty, w, h )
register u_char *dst;
u_int dstw, dsth, w, h;
int dstx, dsty;
{
subimage_set(0,dst,dstw,dsth,dstx,dsty,w,h);
}

/***********************************************************************/

subimage_ones ( dst, dstw, dsth, dstx, dsty, w, h )
register u_char *dst;
u_int dstw, dsth, w, h;
int dstx, dsty;
{
subimage_set(~0,dst,dstw,dsth,dstx,dsty,w,h);
}

/***********************************************************************/

subimage_invert ( dst, dstw, dsth, dstx, dsty, w, h )
register u_char *dst;
u_int dstw, dsth, w, h;
int dstx, dsty;
{
static int first=1;
static u_char lut[LUTSIZE];
register u_int i;

if (first) {
	first = 0;
	for (i=0; i<LUTSIZE; i++)
		lut[i] = ~ (u_char) i;
}
subimage_lookup(lut,dst,dstw,dsth,dstx,dsty,w,h);
}
