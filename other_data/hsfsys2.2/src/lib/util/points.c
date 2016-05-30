/*
# proc: malloc_points - allocates a list of points.
# proc:
# proc: add_point - appends a specified point to the end of the list.
# proc:
# proc: fill_points_down - appends points to list filling a range of
# proc:                    y-values downwards with same x-value.
# proc: fill_points_up - appends points to list filling a range of
# proc:                    y-values upwards with same x-value.
# proc: nearestpoint - given a point determines which point in a specified
# proc:                list is closest.
*/

#include <values.h>
#include <defs.h>

/*******************************************************************/
malloc_points(px, py, pn, pa)
int **px, **py, *pn, *pa;
{
   malloc_int(px, CHUNKS, "malloc_points : malloc : px");
   malloc_int(py, CHUNKS, "malloc_points : malloc : py");
   *pn = 0;
   *pa = CHUNKS;
}

/*******************************************************************/
add_point(x, y, lx, ly, ln, la)
int x, y;
int **lx, **ly, *ln, *la;
{
   if((*ln) >= (*la)){
      (*la) += CHUNKS;
      realloc_int(lx, (*la), "add_point : realloc : lx");
      realloc_int(ly, (*la), "add_point : realloc : ly");
   }
   (*lx)[*ln] = x;
   (*ly)[*ln] = y;
   (*ln)++;
}

/*******************************************************************/
fill_points_down(x, fy, ty, lx, ly, ln, la)
int x, fy, ty;
int **lx, **ly, *ln, *la;
{
   int y;

   for(y = fy; y <= ty; y++)
      add_point(x, y, lx, ly, ln, la);
}

/*******************************************************************/
fill_points_up(x, fy, ty, lx, ly, ln, la)
int x, fy, ty;
int **lx, **ly, *ln, *la;
{
   int y;

   for(y = fy; y >= ty; y--)
      add_point(x, y, lx, ly, ln, la);
}

/*************************************************************************/
nearestpoint(min_i, min_dist, p, px, py, pnum)
int *min_i;
float *min_dist;
int p, *px, *py, pnum;
{
   int i;
   float dist, distance();

   *min_i = -1;
   *min_dist = MAXFLOAT;
   for(i = 0; i < pnum; i++){
      if(i != p){
         dist = distance((float)px[p], (float)py[p], (float)px[i], (float)py[i]);
         if(dist < *min_dist){
            *min_dist = dist;
            *min_i = i;
         }
      }
   }
}

