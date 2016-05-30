typedef struct mlpparamstruct{
   char **class_map;
   int  ninps;
   int  nhids;
   int  nouts;
   char acfunc_hids;
   char acfunc_outs;
   float *weights;
} MLP_PARAM;
