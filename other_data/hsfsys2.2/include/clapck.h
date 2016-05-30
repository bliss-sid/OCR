extern float slamch_(char *);
extern int ilaenv_(int *, char *, char *, int *, int *, 
	           int *, int *, long, long);

extern int sswap_(const int * const,
                 float *, const int * const,
                 float *, const int * const);

extern void ssyevx_(const char * const,
        const char * const,
        const char * const,
        const int * const, float *,
        const int * const,
        const float *,
        const float *,
        const int * const,
        const int * const,
        const float * const,
        int *,
        float *,
        float *, const int * const,
        float *, const int * const,
        int   *,
        int   *,
        int   *);

void diag_mat(const int, float *, int, float **, float **,
              int **, int *);

void la_eigen(const int, int *, float **, float **, float *,
              const int);
