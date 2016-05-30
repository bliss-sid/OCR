/*
# proc: csopiwh - checks for setting of parms in weights-file header.
When there is
an input weights-file, these parms will get their values set from its
header, and they should not be set in the specfile.  (Only warnings.
If any of these parms are set in the specfile, those values will not
be used.) */

#include <mlp/parms.h>

void
csopiwh(parms)
PARMS *parms;
{
  char str[150];

  if(parms->purpose.ssl.set_tried) {
    sprintf(str, "purpose has been set (line %d); that value \
will not be used (the purpose will be read from the weights file).",
      parms->purpose.ssl.linenum);
    eb_cat_w(str);
  }
  if(parms->ninps.ssl.set_tried) {
    sprintf(str, "ninps has been set (line %d); that value \
will not be used (ninps will be read from the weights file).",
      parms->ninps.ssl.linenum);
    eb_cat_w(str);
  }
  if(parms->nhids.ssl.set_tried) {
    sprintf(str, "nhids has been set (line %d); that value \
will not be used (nhids will be read from the weights file).",
      parms->nhids.ssl.linenum);
    eb_cat_w(str);
  }
  if(parms->nouts.ssl.set_tried) {
    sprintf(str, "nouts has been set (line %d); that value \
will not be used (nouts will be read from the weights file).",
      parms->nouts.ssl.linenum);
    eb_cat_w(str);
  }
  if(parms->acfunc_hids.ssl.set_tried) {
    sprintf(str, "acfunc_hids has been set (line %d); that value \
will not be used (acfunc_hids will be read from the weights file).",
      parms->acfunc_hids.ssl.linenum);
    eb_cat_w(str);
  }
  if(parms->acfunc_outs.ssl.set_tried) {
    sprintf(str, "acfunc_outs has been set (line %d); that value \
will not be used (acfunc_outs will be read from the weights file).",
      parms->acfunc_outs.ssl.linenum);
    eb_cat_w(str);
  }
}
