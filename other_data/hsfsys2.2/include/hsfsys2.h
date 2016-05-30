#define GEN_REG_POINTS "/usr/local/hsfsys2/tmplt/hsfgreg.pts"
#define ZOOMFLAG       4

#define DIGIT_EVT_REL2 "/usr/local/hsfsys2/weights/mlp/digit/h6_d.evt"
#define DIGIT_WT_REL2 "/usr/local/hsfsys2/weights/mlp/digit/trn_6.wts"
#define DIGIT_SET_REL2 "/usr/local/hsfsys2/weights/mlp/digit/d.set"

#define LOWER_EVT_REL2 "/usr/local/hsfsys2/weights/mlp/lower/h46_l.evt"
#define LOWER_WT_REL2 "/usr/local/hsfsys2/weights/mlp/lower/trn_6.wts"
#define LOWER_SET_REL2 "/usr/local/hsfsys2/weights/mlp/lower/l.set"

#define UPPER_EVT_REL2 "/usr/local/hsfsys2/weights/mlp/upper/h46_u.evt"
#define UPPER_WT_REL2 "/usr/local/hsfsys2/weights/mlp/upper/trn_6.wts"
#define UPPER_SET_REL2 "/usr/local/hsfsys2/weights/mlp/upper/u.set"

#define CONST_EVT_REL2 "/usr/local/hsfsys2/weights/mlp/const/h46_ul.evt"
#define CONST_WT_REL2 "/usr/local/hsfsys2/weights/mlp/const/trn_6.wts"
#define CONST_SET_REL2 "/usr/local/hsfsys2/weights/mlp/const/ul.set"

#define time_accum(_var_); \
      if(_time_){ \
         _secs_ = ((unsigned long)ticks() - _timer_)/(float)ticksPerSec(); \
         _var_ += _secs_; \
      } \
   }

#define time2_accum(_var_); \
      if(_time_){ \
         _secs2_ = ((unsigned long)ticks() - _timer2_)/(float)ticksPerSec(); \
         _var_ += _secs2_; \
      } \
   }

extern int _total_forms_;
extern float _run_init_;
extern float _form_load_;
extern float _form_reg_;
extern float _field_iso_;
extern float _field_seg_;
extern float _char_norm_;
extern float _char_feat_;
extern float _char_class_;
extern float _field_spell_;
extern float _total_time_;
