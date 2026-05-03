#include <stdbool.h>

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern SDL_Surface *surface;
extern SDL_Texture *texture;
extern int w, h, vps;
extern long frame;
extern bool downface;

void mines_drawBackground();
void mines_3numbers(int num, int x, int y);
void mines_face(int emotion, int x, int y);
void mines_cells(int x, int y);

#define SCALE 2

// lols, this is what happens when you try to avoid magic numbers
#define THENUMBER3 3

#define NUMWIDTH  13
#define NUMHEIGHT 23

#define CELL_SIZE 16
#define CELLX 10
#define CELLY 52

// face
#define FACE_SIZE 26
// emotions
#define FACE_HAPPY 0
#define FACE_PRESS 1
#define FACE_SHOCK 2
#define FACE_DEATH 3
#define FACE_COOOL 4

// actual game logics
extern int face;
extern int flags; // calculated
extern int deadcx, deadcy;
extern long beginFrame;
extern bool begun, died;

extern int GRIDWIDTH, GRIDHEIGHT;
extern int HOWMANY;

////////////////////////////////
// bits and flags             //
// 0 - is there a mine        //
// 1 - should we reveal it    //
// 2 - is it unchosen         //
// 3 - is it flagged          //
// 4 - is it question flagged //
////////////////////////////////
extern unsigned char** grid;

long mines_count();
long mines_cellsleft();
int mines_around(int x, int y);
void mines_reveal(bool flag);
void mines_init();
void mines_logic(int mx, int my, uint32_t mstate, uint32_t mUpMask, uint32_t mDownMask);
void mines_fill(int x, int y);
