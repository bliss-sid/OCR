/*
# proc: eb_cat_w - Puts "Warning " in front of provided string, formats the
# proc:            result, catenates that onto the hidden error-messages buffer,
# proc:            and sets the hidden warning-exists flag.
*/

void
eb_cat_w(str)
char str[];
{
  char errstr[500], errstr2[500];

  sprintf(errstr2, "Warning: %s", str);
  strm_fmt(errstr2, errstr);
  eb_cat(errstr);
  is_w_set();
}
