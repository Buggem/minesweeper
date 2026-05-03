#include <SDL2/SDL.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "mines.h"


void mines_drawBackground() {
    SDL_RenderClear(renderer);

    for(int i = 0; i < (w / 10)+1; i++)
    {
        SDL_Rect srcrect = {40, 81, 10, 10}, dstrect = {i*10, 0, 10, 10};
        SDL_RenderCopy(renderer, texture, &srcrect, &dstrect);
    }
    for(int i = 0; i < (w / 10)+1; i++)
    {
        SDL_Rect srcrect = {40, 81, 10, 10}, dstrect = {i*10, h-10, 10, 10};
        SDL_RenderCopy(renderer, texture, &srcrect, &dstrect);
    }
    for(int i = 0; i < (h / 10)+1; i++)
    {
        SDL_Rect srcrect = {134, 39, 10, 10}, dstrect = {0, i*10, 10, 10};
        SDL_RenderCopy(renderer, texture, &srcrect, &dstrect);
    }
    for(int i = 0; i < (h / 10)+1; i++)
    {
        SDL_Rect srcrect = {134, 39, 10, 10}, dstrect = {w-10, i*10, 10, 10};
        SDL_RenderCopy(renderer, texture, &srcrect, &dstrect);
    }
    for(int i = 0; i < (w / 10)+1; i++)
    {
        SDL_Rect srcrect = {40, 81, 10, 10}, dstrect = {i*10, 42, 10, 10};
        SDL_RenderCopy(renderer, texture, &srcrect, &dstrect);
    }
    {
        SDL_Rect srcrect = {56, 81, 10, 10}, dstrect = {0, 42, 10, 10};
        SDL_RenderCopy(renderer, texture, &srcrect, &dstrect);
    }
    {
        SDL_Rect srcrect = {66, 81, 10, 10}, dstrect = {w-10, 42, 10, 10};
        SDL_RenderCopy(renderer, texture, &srcrect, &dstrect);
    }
    {
        SDL_Rect srcrect = {0, 81, 10, 10}, dstrect = {0, 0, 10, 10};
        SDL_RenderCopy(renderer, texture, &srcrect, &dstrect);
    }
    {
        SDL_Rect srcrect = {10, 81, 10, 10}, dstrect = {w-10, 0, 10, 10};
        SDL_RenderCopy(renderer, texture, &srcrect, &dstrect);
    }
    {
        SDL_Rect srcrect = {20, 81, 10, 10}, dstrect = {0, h-10, 10, 10};
        SDL_RenderCopy(renderer, texture, &srcrect, &dstrect);
    }
    {
        SDL_Rect srcrect = {30, 81, 10, 10}, dstrect = {w-10, h-10, 10, 10};
        SDL_RenderCopy(renderer, texture, &srcrect, &dstrect);
    }
}

void mines_3numbers(int num, int x, int y) {
    if(num > 999) num = 999;
    if(num < -99) num = -99;
    int nums[THENUMBER3] = {0, 0, 0};
    for(int i = 0; i < THENUMBER3; i++) {
        nums[i] = (num / (long)powl(10, THENUMBER3-1-i)) % 10;
        nums[i] = nums[i] < 0 ? -nums[i] : nums[i];
    }
    if(num < 0) nums[0] = 10; // 10 is the -
    //printf("%d = {%d, %d, %d}\n", num, nums[0], nums[1], nums[2]);

    // draw the numbers
    for(int i = 0; i < THENUMBER3; i++)
    {
        SDL_Rect srcrect = {nums[i] * NUMWIDTH, 0, NUMWIDTH, NUMHEIGHT}, dstrect = {x + i * NUMWIDTH, y, NUMWIDTH, NUMHEIGHT};
        SDL_RenderCopy(renderer, texture, &srcrect, &dstrect);
    }
}

void mines_face(int emotion, int x, int y) {
    {
        SDL_Rect srcrect = {FACE_SIZE * emotion, 55, 26, 26}, dstrect = {x, y, 26, 26};
        SDL_RenderCopy(renderer, texture, &srcrect, &dstrect);
    }
}

void mines_cells(int x, int y) {
    for(int i = 0; i < GRIDWIDTH; i++)
    for(int j = 0; j < GRIDHEIGHT; j++) {
        bool gobacktoflag = false;
        SDL_Rect srcrect = {0, NUMHEIGHT + CELL_SIZE, CELL_SIZE, CELL_SIZE},
                 dstrect = {x + i * CELL_SIZE, y + j * CELL_SIZE, CELL_SIZE, CELL_SIZE};

        backtoflag:
        if((grid[i][j] & 8) != 0) {
            if((grid[i][j] & 1) == 0 && died) srcrect.x = CELL_SIZE * 3;
            else srcrect.x = CELL_SIZE;
            srcrect.y = NUMHEIGHT + CELL_SIZE;
        } else if((grid[i][j] & 16) != 0) {
            srcrect.x = CELL_SIZE * 5;
            srcrect.y = NUMHEIGHT + CELL_SIZE;
        }

        if((grid[i][j] & 4) == 0 && !gobacktoflag) {
            srcrect.x = 0;
            srcrect.y = NUMHEIGHT;
            if((grid[i][j] & 16) != 0) {
                srcrect.x = CELL_SIZE * 6;
                srcrect.y = NUMHEIGHT + CELL_SIZE;
            }
            if((grid[i][j] & 2) != 0) {
                if((grid[i][j] & 1) == 0) {
                    srcrect.x = mines_around(i, j) * CELL_SIZE;
                    srcrect.y = NUMHEIGHT;
                } else if((grid[i][j] & 8) == 0 && (grid[i][j] & 16) == 0) {
                    srcrect.x = (i == deadcx && j == deadcy ? 2 : 4) * CELL_SIZE;
                    srcrect.y = NUMHEIGHT + CELL_SIZE;
                } else if((grid[i][j] & 8) != 0 || (grid[i][j] & 16) != 0) {
                    gobacktoflag = true;
                    goto backtoflag;
                }
            }
        }

        SDL_RenderCopy(renderer, texture, &srcrect, &dstrect);
    }
}
// actual minesweeper logics
int flags = 0;
int face = FACE_HAPPY;
int deadcx = -1, deadcy = -1;
bool begun = false, died = false;

long mines_count() {
    long minescount = 0;
    for(int i = 0; i < GRIDWIDTH; i++)
        for(int j = 0; j < GRIDHEIGHT; j++) {
            if((grid[i][j] & 1) != 0) minescount++;
        }
    //printf("%d\n",minescount);
    return minescount;
}

long mines_cellsleft() {
    long cellsleft = GRIDWIDTH*GRIDHEIGHT-mines_count();
    for(int i = 0; i < GRIDWIDTH; i++)
        for(int j = 0; j < GRIDHEIGHT; j++) {
            if((grid[i][j] & 2) != 0 && ((grid[i][j] & 1) == 0)) cellsleft--;
        }
    return cellsleft;
}

int mines_around(int x, int y) {
    int minescount = 0;

    if((x)   >= 0 && (y+1) >= 0 && (x)   < GRIDWIDTH && (y+1) < GRIDHEIGHT) if((grid[x]  [y+1] & 1) != 0) minescount++;
    if((x-1) >= 0 && (y+1) >= 0 && (x-1) < GRIDWIDTH && (y+1) < GRIDHEIGHT) if((grid[x-1][y+1] & 1) != 0) minescount++;
    if((x+1) >= 0 && (y+1) >= 0 && (x+1) < GRIDWIDTH && (y+1) < GRIDHEIGHT) if((grid[x+1][y+1] & 1) != 0) minescount++;

    //if((x)   >= 0 && (y)   >= 0 && (x)   < GRIDWIDTH && (y)   < GRIDHEIGHT) if((grid[x]  [y]   & 1) != 0) minescount++;
    if((x-1) >= 0 && (y)   >= 0 && (x-1) < GRIDWIDTH && (y)   < GRIDHEIGHT) if((grid[x-1][y]   & 1) != 0) minescount++;
    if((x+1) >= 0 && (y)   >= 0 && (x+1) < GRIDWIDTH && (y)   < GRIDHEIGHT) if((grid[x+1][y]   & 1) != 0) minescount++;

    if((x)   >= 0 && (y-1) >= 0 && (x)   < GRIDWIDTH && (y-1) < GRIDHEIGHT) if((grid[x]  [y-1] & 1) != 0) minescount++;
    if((x-1) >= 0 && (y-1) >= 0 && (x-1) < GRIDWIDTH && (y-1) < GRIDHEIGHT) if((grid[x-1][y-1] & 1) != 0) minescount++;
    if((x+1) >= 0 && (y-1) >= 0 && (x+1) < GRIDWIDTH && (y-1) < GRIDHEIGHT) if((grid[x+1][y-1] & 1) != 0) minescount++;

    return minescount;
}

void mines_reveal(bool flag) {
    for(int i = 0; i < GRIDWIDTH; i++)
        for(int j = 0; j < GRIDHEIGHT; j++) {
            if((grid[i][j] & 1) != 0) {
                grid[i][j] &= ~4;
                grid[i][j] |= 2;
                if(flag) {
                    grid[i][j] &= ~16;
                    grid[i][j] |= 8;
                }
            }
        }
}

void mines_init() {
    deadcx = -1; deadcy = -1;
    flags = 0;
    begun = false;
    beginFrame = frame;
    died = false;
    face = FACE_HAPPY;

    for(int i = 0; i < GRIDWIDTH; i++)
        memset(grid[i], 4, GRIDHEIGHT * sizeof(unsigned char));

    long minescount = 0;
    while(minescount < HOWMANY) {
        int rx = ((double)rand()*GRIDWIDTH/RAND_MAX), ry = ((double)rand()*GRIDHEIGHT/RAND_MAX);
        if((grid[rx][ry] & 1) == 0) grid[rx][ry] |= 1; // place a mine
        minescount = mines_count();
    }
}
bool run = false, dontrechoose = false;
void mines_logic(int mx, int my, uint32_t mstate, uint32_t mUpMask, uint32_t mDownMask) {
    if(died) return;
    if(mines_cellsleft() == 0) {
        // "dead" - (you win)
        mines_reveal(true);
        face = FACE_COOOL;
        begun = false;
        died = true;
        return;
    }
    run = !run;
    if((mx >= CELLX && mx < CELLX+GRIDWIDTH  * CELL_SIZE) &&
       (my >= CELLY && my < CELLY+GRIDHEIGHT * CELL_SIZE)
    ) {
        int cellx = (mx - CELLX) / CELL_SIZE,
            celly = (my - CELLY) / CELL_SIZE;

        // don't even try to get me to explain this awful code
        if((grid[cellx][celly] & 8) == 0 && !downface) {
            if(run && (mstate & SDL_BUTTON(1)) != 0) {
                dontrechoose = (grid[cellx][celly] & 4) != 0;
                grid[cellx][celly] = grid[cellx][celly] & ~4; // unchosen[cellx][celly] = false;
            }
            else if(!run && (mstate & SDL_BUTTON(1)) != 0) {
                if(dontrechoose) grid[cellx][celly] |= 4; // unchosen[cellx][celly] = true;
            }
        }
        if(!run) return;

        // if it works it works lol
        if((mUpMask & SDL_BUTTON(1)) != 0 && (grid[cellx][celly] & 8) == 0 && !downface) {
            //grid[cellx][celly] = grid[cellx][celly] & ~4; // unchosen[cellx][celly] = false;
            //grid[cellx][celly] |= 2; // flags mines as "revealed"

            if((grid[cellx][celly] & 1) != 0 && begun) {
                // dead
                deadcx = cellx; deadcy = celly;
                mines_reveal(false);
                begun = false;
                died = true;
                face = FACE_DEATH;

                mines_fill(cellx, celly);
            } else {
                if((grid[cellx][celly] & 1) != 0) {
                    grid[cellx][celly] &= ~1;
                    long nexi = 0;
                    while(1) {
                        if(nexi >= GRIDWIDTH*GRIDHEIGHT) break;
                        int nexx = nexi % GRIDWIDTH, nexy = nexi / GRIDWIDTH;

                        if((grid[nexx][nexy] & 1) == 0 && !(nexx == cellx && nexy == celly)) break;
                        nexi++;
                    }
                    grid[nexi % GRIDWIDTH][nexi / GRIDWIDTH] |= 1;
                }
                mines_fill(cellx, celly);
                begun = true;
            }
        }

        if((mDownMask & SDL_BUTTON(3)) != 0 && (grid[cellx][celly] & 2) == 0) {
            if((grid[cellx][celly] & 8) != 0) {
                grid[cellx][celly] &= ~8;
                grid[cellx][celly] |= 16;
                flags--;
            } else if((grid[cellx][celly] & 16) != 0) {
                grid[cellx][celly] &= ~(8 | 16);
            } else {
                grid[cellx][celly] |= 8;
                flags++;
            }
        } else if((grid[cellx][celly] & 2) != 0) {
            if((grid[cellx][celly] & 8) != 0) flags--;
            grid[cellx][celly] &= ~(8 | 16);
        }
    }
}

void mines_fill(int x, int y) {
    if(x < 0 || x >= GRIDWIDTH || y < 0 || y >= GRIDHEIGHT) return;
    if((grid[x][y] & 2) != 0 || (grid[x][y] & 8) != 0) return;
    grid[x][y] = grid[x][y] & ~4; // unchosen[cellx][celly] = false;
    grid[x][y] |= 2;
    if ((grid[x][y] & 1) == 0 && mines_around(x, y) == 0) {
        mines_fill( x,   y-1 );
        mines_fill( x-1, y-1 );
        mines_fill( x+1, y-1 );


        mines_fill( x-1, y   );
        mines_fill( x+1, y   );

        mines_fill( x,   y+1 );
        mines_fill( x-1, y+1 );
        mines_fill( x+1, y+1 );
    } else {
        return;
    }
}
