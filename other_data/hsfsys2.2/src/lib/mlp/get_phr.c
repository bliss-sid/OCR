/*
# proc: get_phr - Gets the next unignorable phrase from a specfile, and the
# proc:           number of the line in which it occurs.
A phrase is the result of
getting the next sequence delimited by start of file, end of file,
semicolon, and newline, and then trimming any leading and trailing
spaces and tabs.  A phrase that is empty or that consists of a legal
parm-name without an accompanying value, is ignored by this routine,
causing it to continue scanning.  A phrase that causes a return is one
of the following: a pair of words, separated from each other by
non-newline whitespace characters; or "newrun" or "NEWRUN"; or an
illegal phrase (not "newrun" or "NEWRUN", not a legal parm-name by
itself, and not a pair of words).  A return is also caused by the
exhaustion of the rest of the specfile without the finding of another
unignorable phrase.

The routine ignores comments delimited as in the C language, i.e.
with slash asterisk and asterisk slash.  (Newlines in comments affect
the returned line-numbers, because otherwise the line-numbers would be
of no value in typical editors; but newlines in comments do not affect
the delimiting of phrases, and therefore although newline is usually a
phrase-delimiter, a phrase (legal or illegal) can contain a comment
that contains a newline.  The line-number this routine returns for
such a multi-line phrase is that of the first line used by the
phrase.)

Input arg:
  fp: FILE pointer of specfile.

Output args:
  word1, word2: The pair of words found, if that is what was found.
    Buffers must be allocated by caller.
  illegal_phrase: The illegal phrase found, if that is what was found.
    Buffer must be allocated by caller.
  linenum: The line-number (starting at 1) at which an unignorable
    phrase (legal or not) was found, if one was found.

Return value (these names are defined in get_phr.h):
  WORD_PAIR: A phrase was found that is a word-pair.  The words are in
    word1 and word2, and the line-number at which they were found is
    *linenum.
  NEWRUN: A phrase was found that is either "newrun" or "NEWRUN".  The
    line-number at which it was found is *linenum.
  ILLEGAL_PHRASE: A phrase was found that is not a NEWRUN, is not a
    legal parm-name, and is not a pair of words.  The line-number at
    which it was found is *linenum.
  FINISHED: The rest of the specfile was exhausted without another
    unignorable phrase being found.
*/

#include <stdio.h>
#include <string.h>
#include <mlp/defs.h>
#include <mlp/get_phr.h>

char
get_phr(fp, word1, word2, illegal_phrase, linenum)
FILE *fp;
char word1[], word2[], illegal_phrase[];
int *linenum;
{
  char phrasity[500], *p, c, *phrase, barf[500], c_ret;
  int will_start_new_phrasity, ln, ln_yow, i_ret;
  static int nomore_noncomment_chars = FALSE;
  char got_nc_c(), lgl_pnm();

  if(nomore_noncomment_chars) {
    nomore_noncomment_chars = FALSE;
    return FINISHED;
  }

  /* Scan until either finding another unignorable phrase, or
  exhausting the specfile.  An empty phrase, or one that is a legal
  parm-name without an accompanying value, is ignored, causing
  scanning to continue. */
  while(1) {

    /* Find next phrasity: delimeters are start of file, end of file,
    semicolon, and newline. */
    for(p = phrasity, will_start_new_phrasity = TRUE; ;) {
      c_ret = got_nc_c(fp, &c, &ln);
      if(!c_ret || strchr("\n;", c)) {
	if(!c_ret)
	  nomore_noncomment_chars = TRUE;
	*p = '\0';
	break;
      }
      else {
	if(will_start_new_phrasity) {
	  will_start_new_phrasity = FALSE;
	  ln_yow = ln;
	}
	*p++ =  c;
      }
    }

    if(sscanf(phrasity, "%s", barf) != 1) {
      /* Phrasity is either empty, or contains nothing but whitespace
      characters.  Ignore it and continue scanning, except that if
      nomore_noncomment_chars is TRUE, there is nothing important
      after this vacuous phrasity, and so, finished. */
      if(nomore_noncomment_chars) {
	nomore_noncomment_chars = FALSE;
	return FINISHED;
      }
      continue;
    }

    /* Trim leading and trailing spaces and tabs, if any, from the
    phrasity, producing the phrase. */
    for(phrase = phrasity; strchr(" \t\r", *phrase); phrase++);
    for(p = phrase + strlen(phrase) - 1; strchr(" \t\r", *p); p--);
    *++p = '\0';

    /* Phrase is empty; ignore it and continue scanning. */
    if(!strcmp(phrase, ""))
      continue;

    *linenum = ln_yow;

    i_ret = sscanf(phrase, "%s %s %s", word1, word2, barf);
    if(i_ret == 1) { /* phrase consists of one word */
      
      if(!strcmp("newrun", phrase) || !strcmp("NEWRUN", phrase))
	return NEWRUN;

      if(lgl_pnm(phrase))
	/* Phrase is a legal parmname without a value; ignore it and
        continue scanning.  (Lets user temporarily disable a
        parm-setting by deleting just the value.) */
	continue;

      /* the word probably has a typo */
      strcpy(illegal_phrase, phrase);
      return ILLEGAL_PHRASE;
    }
    else if(i_ret == 2)
      return WORD_PAIR;
    else { /* i_ret == 3: phrase has >= 3 words, and that is illegal */
      strcpy(illegal_phrase, phrase);
      return ILLEGAL_PHRASE;
    }
  }
}
