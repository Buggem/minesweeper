#include <SDL2/SDL.h>
#include <string.h>
#include <stdlib.h>
#include "config.h"
#include "mines.h"


int GRIDWIDTH = 9, GRIDHEIGHT = 9;
int HOWMANY = 10;
unsigned char** grid;

int config_game(int argc, char* argv[])
{
    for(int i = 0; i < argc; i++) {
        if(strcmp(argv[i], "-m") == 0) {
            if(i+1 < argc) i++;
            else {
                printf("-m assumes the number of mines following, i.e. `./minesweeper -m 10` for 10 mines\n");
                return 1;
            }
            for(char* j = argv[i]; (j-argv[i]) < strlen(argv[i]); j++)
                if(!isdigit(*j)) {
                    printf("-m assumes a NUMBER\n");
                    return 1;
                }
            HOWMANY = atoi(argv[i]);
        }
    }
    if(HOWMANY > (GRIDWIDTH-1) * (GRIDHEIGHT-1)) HOWMANY = (GRIDWIDTH-1) * (GRIDHEIGHT-1);

    grid = (unsigned char**)malloc(GRIDWIDTH*sizeof(unsigned char*));
    //printf("%p\n", grid);

    for(int i = 0; i < GRIDWIDTH; i++) {
        grid[i] = (unsigned char*)malloc(GRIDHEIGHT*sizeof(unsigned char));
        //printf("%d %p\n", i, grid[i]);
    }
    return 0;
}
