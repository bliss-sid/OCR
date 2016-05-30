/*
# proc: store_phrase_dictionary - writes the contents of the given phrase
# proc:                           dictionary (mfs structure) to the specified
# proc:                           filename.
*/

#include <mfs.h>

store_phrase_dictionary(filename, phrases)
char *filename;
MFS *phrases;
{
   writemfsfile(filename, phrases);
}
