#define CORNERS		4
#define MAXBOX		100

struct vertx{
             int x,y;
            };
struct boxx{
          struct vertx vertex[CORNERS];
          };
typedef struct hsform{
          int num;
          struct boxx box[MAXBOX];
          }HSFORM;
