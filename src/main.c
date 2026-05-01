#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdint.h>
#include "mines.h"
SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *texture;
int w, h, vps;
long frame, beginFrame;

int main(int argc, char* args[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");

    mines_init();

    w = 20 + GRIDWIDTH  * CELL_SIZE;
    h = 64 + GRIDHEIGHT * CELL_SIZE;

    window = SDL_CreateWindow("Minesweeper", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w * SCALE, h * SCALE, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_SetRenderDrawColor(renderer, 189, 189, 189, 255);
    SDL_RenderSetLogicalSize(renderer, w, h);

    SDL_Surface *sprite = IMG_Load("mines.png");
    texture = SDL_CreateTextureFromSurface(renderer, sprite);
    SDL_FreeSurface(sprite);

    SDL_Surface *icon = IMG_Load("icon.png");
    SDL_SetWindowIcon(window, icon);

    frame = 0;
    // get vps
    {
        SDL_DisplayMode mode;
        SDL_GetCurrentDisplayMode(SDL_GetWindowDisplayIndex(window), &mode);
        vps = mode.refresh_rate;
        printf("%d fps\n", vps);
    }

    // loop
    while(true) {
        SDL_Event event;
        uint32_t mUpMask = 0, mDownMask = 0;
        bool quit = false;
        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {
                quit = true;
                break;
            }
            if(event.type == SDL_MOUSEBUTTONUP) {
                mUpMask   |= SDL_BUTTON(event.button.button);
            } else if(event.type == SDL_MOUSEBUTTONDOWN) {
                mDownMask |= SDL_BUTTON(event.button.button);
            }
        }
        if(quit) {
            break;
        }

        frame++;
        if(!begun) beginFrame ++;

        int mx, my;
        uint32_t mstate = SDL_GetMouseState(&mx, &my);
        mx /= SCALE;
        my /= SCALE;

        mines_logic(mx, my, mstate, mUpMask, mDownMask);
        mines_drawBackground();
        mines_3numbers(mines_count()-flags, 16, 14);
        mines_3numbers((frame-beginFrame)/vps, w-(NUMWIDTH*THENUMBER3+16), 14);
        int facex = (w-FACE_SIZE)/2, facey = 13;
        if((mx >= facex && mx < facex+FACE_SIZE) && (my >= facey && my < facey+FACE_SIZE)) {
            mines_face((mstate & SDL_BUTTON(1)) != 0 ? FACE_PRESS : face, facex, facey);
            if((mUpMask & SDL_BUTTON(1)) != 0) {
                mines_init();
            }
        }
        else
            mines_face(face, facex, facey);

        mines_cells(CELLX, CELLY);

        mines_logic(mx, my, mstate, mUpMask, mDownMask);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    return 0;
}
