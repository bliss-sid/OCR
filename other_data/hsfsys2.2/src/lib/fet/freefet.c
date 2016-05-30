/*
# proc: freefet - deallocates the memory for an fet structure.
# proc:
*/

#include <fet.h>

void freefet(fet)
FET *fet;
{
  int item;
  for (item=0;item<fet->num;item++)
    {
      free (fet->names[item]);
      free (fet->values[item]);
    }
  free((char *)fet->names);
  free((char *)fet->values);
  free(fet);
}
