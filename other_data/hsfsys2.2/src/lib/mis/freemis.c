/*
# proc: freemis - deallocates the memory associated with the specified
# proc:           mis structure.
# proc: freemishdr - deallocates the header associated with the specified
# proc:              mis structure. Only used after a call to readmishdr.
*/

#include <ihead.h>
#include <mis.h>

void freemis(mis)
MIS *mis;
{
  free((char *)mis->head);
  free((char *)mis->data);
  free((char *)mis);
}

void freemishdr(mis)
MIS *mis;
{
  free((char *)mis->head);
  free((char *)mis);
}
