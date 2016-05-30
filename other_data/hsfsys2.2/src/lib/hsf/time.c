/*
# proc: print_time_report - generates a timing report and printes it to the
# proc:                     specified file.
*/

/* global variables for accumulating timing statistics in the 2nd */
/* release of hsfsys */

int _total_forms_ = 0;
float _run_init_ = 0.0;
float _form_load_ = 0.0;
float _form_reg_ = 0.0;
float _field_iso_ = 0.0;
float _field_seg_ = 0.0;
float _char_norm_ = 0.0;
float _char_feat_ = 0.0;
float _char_class_ = 0.0;
float _field_spell_ = 0.0;
float _total_time_ = 0.0;

#include <stdio.h>
#include <hsfsys2.h>

print_time_report(timefile)
char *timefile;
{
   FILE *fp;

   if((fp = fopen(timefile, "wb")) == NULL)
      syserr("print_time_report", "fopen", timefile);

   fprintf(fp, "Run Initialize :            \t%6.2f\t%5.1f\n", _run_init_,
           (_run_init_ / _total_time_ * 100.0));
   fprintf(fp, "Form Load :                 \t%6.2f\t%5.1f\n", _form_load_,
           (_form_load_ / _total_time_ * 100.0));
   fprintf(fp, "Form Register :             \t%6.2f\t%5.1f\n", _form_reg_,
           (_form_reg_ / _total_time_ * 100.0));
   fprintf(fp, "Field Isolate :             \t%6.2f\t%5.1f\n", _field_iso_,
           (_field_iso_ / _total_time_ * 100.0));
   fprintf(fp, "Field Segment :             \t%6.2f\t%5.1f\n", _field_seg_,
           (_field_seg_ / _total_time_ * 100.0));
   fprintf(fp, "Character Normalize :       \t%6.2f\t%5.1f\n", _char_norm_,
           (_char_norm_ / _total_time_ * 100.0));
   fprintf(fp, "Character Feature Extract : \t%6.2f\t%5.1f\n", _char_feat_,
           (_char_feat_ / _total_time_ * 100.0));
   fprintf(fp, "Character Classify :        \t%6.2f\t%5.1f\n", _char_class_,
           (_char_class_ / _total_time_ * 100.0));
   fprintf(fp, "Field Spell Correct :       \t%6.2f\t%5.1f\n", _field_spell_,
           (_field_spell_ / _total_time_ * 100.0));
   fprintf(fp, "Run Total Time :            \t%6.2f\t%5.1f\n", _total_time_,
           (_total_time_ / _total_time_ * 100.0));
   fprintf(fp, "Run Total Forms :           \t%d\n", _total_forms_);

   fclose(fp);
}
