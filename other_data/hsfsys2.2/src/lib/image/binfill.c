/*
# proc: binary_fill_partial - uses a logical operator to copy pixels from a
# proc:                       location in one binary scanline to a location in
# proc:                       another binary scanline.
# proc: gb - gets a pixel from a binary scanline.
# proc:
# proc: sb - sets a pixel in a binary scanline.
# proc:
*/

/* LINTLIBRARY */

#include <stdio.h>
#include <values.h>
#include <sys/types.h>

#include <binops.h>


binary_fill_partial(op,src,i,dst,j,bits)
int op, i, j, bits;
u_char *src, *dst;
{
int n;
/*
int i_inv;
int j_inv;
int diff;
u_char sc;
u_char src_begin, src_end;
u_char dst_begin, dst_end;

i_inv = BITSPERBYTE - i;
j_inv = BITSPERBYTE - j;

src_begin = mask_begin_1[i];
src_end   = mask_end_1[i_inv];

dst_begin = mask_begin_1[j];
dst_end   = mask_end_1[j_inv];


if (i + bits <= BITSPERBYTE)
	sc = *src << i;
else
	sc = (*src << i) | (*(src+1) >> i_inv);
*/

for (n=0; n<bits; n++) {
	int srcbit, dstbit;

	switch (op) {

		case BINARY_COPY:
			srcbit = gb(src,i+n);
			sb(dst,j+n,srcbit);
			break;

		case BINARY_OR:
			srcbit = gb(src,i+n);
			dstbit = gb(dst,j+n);
			sb(dst,j+n,srcbit|dstbit);
			break;

		case BINARY_AND:
			srcbit = gb(src,i+n);
			dstbit = gb(dst,j+n);
			sb(dst,j+n,srcbit&dstbit);
			break;

		case BINARY_XOR:
			srcbit = gb(src,i+n);
			dstbit = gb(dst,j+n);
			sb(dst,j+n,srcbit^dstbit);
			break;

		default:
			fatalerr("binary_fill_partial","bad operator");
	} /* SWITCH */
} /* FOR */

/*
diff = bits - j;
m = mask_begin_1[(j+bits) % BITSPERBYTE];
switch (op) {

  case BINARY_COPY:
	if (diff > 0) {
		*dst = (*dst & dst_begin) | ((sc >> j) & dst_end);
		dst++;
		*dst = (*dst & ~m) | ((sc << j_inv) & m);
		dst--;
	} else if (diff < 0)
		*dst = (*dst & dst_begin) | 
	else
		*dst = (*dst & dst_begin) | ((sc >> j) & dst_end);
	break;

  case BINARY_OR:
		break;
'
  case BINARY_AND:
		break;

  default:
		fatalerr("binary_fill_partial","bad opcode",(char *)NULL);
}
*/
}

int gb(p,i)
u_char *p;
int i;
{

p += i / BITSPERBYTE;
i %= BITSPERBYTE;
return (*p >> ((BITSPERBYTE - 1) - i)) & 0x1;
}

sb(p,i,v)
u_char *p;
int i, v;
{
u_char m;

p += i / BITSPERBYTE;
i %= BITSPERBYTE;
m = 0x1 << ((BITSPERBYTE - 1) - i);
if (v)
	*p |= m;
else
	*p &= ~m;
}
