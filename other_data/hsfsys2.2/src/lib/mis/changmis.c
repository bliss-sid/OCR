/*
# proc: changemis - overwrites a specified entry in an mis structure with
# proc:             the image passed.
*/

#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <ihead.h>
#include <mis.h>

void changemis(mis, index, image_data)
MIS *mis;
int index;
unsigned char *image_data;
{
  char number[10];
  int size, offset;
  unsigned char *cur_image_pos=NULL;

  if (index > mis->ent_num - 1) {
     sprintf(number, "%d", index);
     fatalerr("changemis","Invalid entry index",number);
  }
  if(mis->ent_num <= 0)
     fatalerr("changemis","Image_Data list is empty",NULL);
  size = SizeFromDepth(mis->entw, mis->enth, mis->misd);
  offset = size * index;
  cur_image_pos = mis->data + offset;
  memcpy(cur_image_pos, image_data, size);
}
