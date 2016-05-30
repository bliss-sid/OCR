/*
# proc: iso_1line_field - isolates and crops a field containing one line of
# proc:                   text given spatial template coordinates for the
# proc:                   field.
# proc: iso_nline_field - isolates and crops a field containing more than one
# proc:                   line of text given spatial template coordinates for
# proc:                   the field.
*/

#include <stdio.h>

/*******************************************************************/
iso_1line_field(field, par_x, par_y, fw, fh, idata, iw, ih, x1, y1, x2, y2)
unsigned char **field, *idata;
int *par_x, *par_y, *fw, *fh;
int iw, ih, x1, y1, x2, y2;
{
   int bfw, bfh;
   unsigned char *bigfield, *cut_exact_image();
   int *bins, blen;
   int start;
   int ty_i, ty_val, by_i, by_val;
   int lx_i, lx_val, rx_i, rx_val;
   int *rlist, *binarize();
   int m_i, m_val;

   if((iw == 0) || (ih == 0)){
      *par_x = 0;
      *par_y = 0;
      *fw = 0;
      *fh = 0;
      return;
   }

   bfw =  x2 - x1 + 1;
   bfh =  y2 - y1 + 1;
   bigfield = cut_exact_image(idata, iw, ih, x1, y1, &bfw, bfh);

   compute_x_hist(bigfield, bfw, bfh, &bins, &blen);
   ramp_int_runs(bins, &rlist, blen, 2);
   find_first_max_forward(rlist, 0, blen, &m_i, &m_val);
   free(rlist);

   start = m_i - (m_val>>1);
   find_first_min_backward(bins, start, 0, &ty_i, &ty_val);
   find_first_min_forward(bins, start, blen, &by_i, &by_val);
   free(bins);

   compute_y_hist(bigfield, bfw, bfh, &bins, &blen);
   find_first_thresh_forward(bins, 0, blen, 10, &lx_i, &lx_val);
   find_first_min_backward(bins, lx_i, 0, &lx_i, &lx_val);
   find_first_thresh_backward(bins, 0, blen, 10, &rx_i, &rx_val);
   find_first_min_forward(bins, rx_i, blen, &rx_i, &rx_val);
   free(bins);

   *fw = rx_i - lx_i;
   *fh = by_i - ty_i;
   lx_i++;
   ty_i++;
   (*field) = cut_exact_image(bigfield, bfw, bfh, lx_i, ty_i, fw, *fh);
   free(bigfield);
   *par_x = x1 + lx_i;
   *par_y = y1 + ty_i;
}

/*******************************************************************/
iso_nline_field(field, par_x, par_y, fw, fh, idata, iw, ih, x1, y1, x2, y2)
unsigned char **field, *idata;
int *par_x, *par_y, *fw, *fh;
int iw, ih, x1, y1, x2, y2;
{
   int bfw, bfh;
   unsigned char *bigfield, *cut_exact_image();
   int *bins, blen;
   int ty_i, ty_val, by_i, by_val;
   int lx_i, lx_val, rx_i, rx_val;

   if((iw == 0) || (ih == 0)){
      *par_x = 0;
      *par_y = 0;
      *fw = 0;
      *fh = 0;
      return;
   }

   bfw =  x2 - x1 + 1;
   bfh =  y2 - y1 + 1;
   bigfield = cut_exact_image(idata, iw, ih, x1, y1, &bfw, bfh);

   compute_x_hist(bigfield, bfw, bfh, &bins, &blen);
   find_first_thresh_forward(bins, 0, blen, 10, &ty_i, &ty_val);
   find_first_min_backward(bins, ty_i, 0, &ty_i, &ty_val);
   find_first_thresh_backward(bins, 0, blen, 10, &by_i, &by_val);
   find_first_min_forward(bins, by_i, blen, &by_i, &by_val);
   free(bins);

   compute_y_hist(bigfield, bfw, bfh, &bins, &blen);
   find_first_thresh_forward(bins, 0, blen, 10, &lx_i, &lx_val);
   find_first_min_backward(bins, lx_i, 0, &lx_i, &lx_val);
   find_first_thresh_backward(bins, 0, blen, 10, &rx_i, &rx_val);
   find_first_min_forward(bins, rx_i, blen, &rx_i, &rx_val);
   free(bins);

   *fw = rx_i - lx_i;
   *fh = by_i - ty_i;
   lx_i++;
   ty_i++;
   (*field) = cut_exact_image(bigfield, bfw, bfh, lx_i, ty_i, fw, *fh);
   free(bigfield);
   *par_x = x1 + lx_i;
   *par_y = y1 + ty_i;
}
