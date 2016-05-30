/*
# proc: fsaso_init - Initialization call Before first call of fsaso.
# proc: fsaso - Fputs's a string both to stderr and to the short outfile.
*/

/* Routines can use the "fsaso" utility routine to succintly write a
string both to stderr and to the short outfile.  Example usages:

  (Before first call of fsaso:)
  fsaso_init(short_outfile);

  (Then, can use fsaso like the following, in any routine:)

  sprintf(str, "foo: %d; bar: %f\n", foo, bar);
  fsaso(str);

  (or:)

  fsaso(already_filled_in_string);

  (or:)

  fsaso("This is a literal string.\n");

CAUTION: Since fsaso.c has fp as its _private_ stream to the short
outfile, it is impossible to produce good results by doing a sequence
of writes to the short outfile IN WHICH some writes use fsaso and
others explicitly use another stream to the short outfile: that will
fail because neither stream will, when it does a write, move the
location pointer in the other stream.  So, if you are going to use
fsaso at all, then _every_ write to the short outfile should use
fsaso, and so every write to the short outfile will have a mirror
write to stderr, which seems good for mlp.  However, it is ok to
interleave fsaso calls with explicit writes to stderr, since stderr is
the same stream no matter what routine is using it, with the same
location pointer.  So, one can do writes to stderr which are not
mirrored as writes to short outfile, and that definitely is a good
thing for mlp, since some progress writes to stderr do not go with a
particular run and hence should not be written into any short outfile:

  fprintf(stderr, "Will do %d runs.\n", nruns);
  ...
  fsaso(string_for_stderr_and_short_outfile);

So, mlp should write its non-run-specific messages directly to
stderr, and should write all other messages, which go with the
current run, using fsaso.
*/

#include <stdio.h>

static FILE *fp = (FILE *)NULL;

/*******************************************************************/

/* fsaso_init: Before first call of fsaso for a new short outfile
(i.e. a new run), call this with the filename. */

void
fsaso_init(short_outfile)
char short_outfile[];
{
  /* If fp is a stream to preceding short_outfile, fclose it. */
  if(fp != (FILE *)NULL)
    fclose(fp);

  if((fp = fopen(short_outfile, "wb")) == (FILE *)NULL)
    syserr("fsaso_init (fsaso.c)", "fopen for writing failed",
      short_outfile);
}

/*******************************************************************/

/* fsaso: Fputs's a string both to stderr and to the short outfile,
also fflushing both streams. */

void
fsaso(str)
char str[];
{
  fputs(str, stderr);
  fflush(stderr);
  fputs(str, fp);
  fflush(fp);
}

/*******************************************************************/
