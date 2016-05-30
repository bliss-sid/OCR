#define TMPLT_DILATE "/usr/local/hsfsys2/tmplt/hsftmplt.d4"
#define TMPLT_FIELDS "/usr/local/hsfsys2/tmplt/hsftmplt.pts"
#define REG_POINTS "/usr/local/hsfsys2/tmplt/hsfreg.pts"

#define DIGIT_EVT   "/usr/local/hsfsys2/weights/pnn/digit/h6_d.evt"
#define DIGIT_PAT   "/usr/local/hsfsys2/weights/pnn/digit/h6_d.pat"
#define DIGIT_MED   "/usr/local/hsfsys2/weights/pnn/digit/h6_d.med"
#define DIGIT_SET   "/usr/local/hsfsys2/weights/pnn/digit/d.set"
#define DIGIT_EVT_SM   "/usr/local/hsfsys2/weights/pnn/digit/h6_d_s.evt"
#define DIGIT_PAT_SM   "/usr/local/hsfsys2/weights/pnn/digit/h6_d_s.pat"
#define DIGIT_MED_SM   "/usr/local/hsfsys2/weights/pnn/digit/h6_d_s.med"
#define DIGIT_SET_SM   "/usr/local/hsfsys2/weights/pnn/digit/d.set"

#define LOWER_EVT   "/usr/local/hsfsys2/weights/pnn/lower/h46_l.evt"
#define LOWER_PAT   "/usr/local/hsfsys2/weights/pnn/lower/h46_l.pat"
#define LOWER_MED   "/usr/local/hsfsys2/weights/pnn/lower/h46_l.med"
#define LOWER_SET   "/usr/local/hsfsys2/weights/pnn/lower/l.set"

#define UPPER_EVT   "/usr/local/hsfsys2/weights/pnn/upper/h46_u.evt"
#define UPPER_PAT   "/usr/local/hsfsys2/weights/pnn/upper/h46_u.pat"
#define UPPER_MED   "/usr/local/hsfsys2/weights/pnn/upper/h46_u.med"
#define UPPER_SET   "/usr/local/hsfsys2/weights/pnn/upper/u.set"

#define CONST_EVT   "/usr/local/hsfsys2/weights/pnn/const/h46_ul.evt"
#define CONST_PAT   "/usr/local/hsfsys2/weights/pnn/const/h46_ul.pat"
#define CONST_MED   "/usr/local/hsfsys2/weights/pnn/const/h46_ul.med"
#define CONST_SET   "/usr/local/hsfsys2/weights/pnn/const/ul.set"
#define CONST_EVT_SM   "/usr/local/hsfsys2/weights/pnn/const/h6_ul_s.evt"
#define CONST_PAT_SM   "/usr/local/hsfsys2/weights/pnn/const/h6_ul_s.pat"
#define CONST_MED_SM   "/usr/local/hsfsys2/weights/pnn/const/h6_ul_s.med"
#define CONST_SET_SM   "/usr/local/hsfsys2/weights/pnn/const/ul.set"

#define PHRASE_DICT  "/usr/local/hsfsys2/dict/const.mfs"

#define RM_LIMIT  3
#define ERR_LIMIT 4

#define PNN_SIGMA_DIGIT 2.0
#define PNN_SIGMA_ALPHA 3.0

#define DO_DICT 1
#define NO_DICT 2

#define FIRST_DIGIT 3
#define LAST_DIGIT  30

#define FIRST_LOWER 31

#define FIRST_UPPER 32

#define FIRST_CONST 33

extern char *optarg;
extern int optind, opterr;

extern int _silent_, _verbose_, _time_;
extern FILE *_tfp_;

#define verbose_print(_func_, _msg_); \
   if((!_silent_) && (_verbose_)) {\
      fprintf(stderr, "%s:\t%s\n", _func_, _msg_); \
      fflush(stderr); \
   }

#define result_print(_fld_, _rslt_); \
   if(!_silent_) {\
      if(_rslt_ == NULL) \
         fprintf(stdout, "\n%s:\n\n", _fld_); \
      else \
         fprintf(stdout, "\n%s:\t%s\n\n", _fld_, _rslt_); \
      fflush(stdout); \
   }

#define init_timer(_root_); \
   {  char _tfile_[80]; \
      if(_time_){ \
         sprintf(_tfile_, "%s.tim", _root_); \
         _tfp_ = fopen(_tfile_, "w"); \
         if(_tfp_ == NULL) \
            syserr("init_time", "fopen", _tfile_); \
      } \
   }

#define close_timer(); \
   if(_time_) \
      fclose(_tfp_);

#define set_timer(); \
   {  unsigned long _timer_; \
      float _secs_; \
      if(_time_) \
         _timer_ = (unsigned long)ticks();

#define set_timer2(); \
   {  unsigned long _timer2_; \
      float _secs2_; \
      if(_time_) \
         _timer2_ = (unsigned long)ticks();

#define time_elapsed(_msg1_, _msg2_); \
      if(_time_){ \
         _secs_ = ((unsigned long)ticks() - _timer_)/(float)ticksPerSec(); \
         fprintf(_tfp_, "%s :\t%s :\t%7.3f\n", _msg1_, _msg2_, _secs_); \
      } \
   }

#define time2_elapsed(_msg1_, _msg2_); \
      if(_time_){ \
         _secs2_ = ((unsigned long)ticks() - _timer2_)/(float)ticksPerSec(); \
         fprintf(_tfp_, "%s :\t%s :\t%7.3f\n", _msg1_, _msg2_, _secs2_); \
      } \
   }

