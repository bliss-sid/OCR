#define MAXWORDLEN 12

#define X1      2.0
#define Y1      0.5
#define X2     12.0
#define Y2      0.4

typedef struct spellstruct {
   char word[MAXWORDLEN+1];
   char lookup[MAXWORDLEN+1];
   float conf;
   int align[MAXWORDLEN+MAXWORDLEN];
   int alen;
} SPELL_LINE;
