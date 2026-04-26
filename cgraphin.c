#include <stdio.h>
#include <./SDL3/SDL.h>
#include <./SDL3/SDL_main.h>

//compile with
/*
cl.exe /I./ cgraphin.c /link /defaultlib:sdl3 && cgraphin.exe
*/
int width, height;
int to_screen_x(int x)
{
    return x+width/2;
}
int to_screen_y(int y)
{
    return -y+height/2;
}

int main(int argc, char* argv[])
{
    SDL_Window* win;
    SDL_Renderer* ren;
    SDL_CreateWindowAndRenderer("cgraphin", 1000, 1000,SDL_WINDOW_TRANSPARENT, &win, &ren);
    SDL_GetWindowSize(win, &width, &height);
    SDL_Event e;
    unsigned char c = 128;
    int x=0,y=0;
    int it=0, j=0;
    int xdir=-1,ydir=1;
    unsigned char bright=255;
    unsigned char r=bright, g=0, b=0;
    SDL_SetRenderDrawColor(ren, 0,0,0,50);
    SDL_RenderClear(ren);
    while(true)
    {  
        while(SDL_PollEvent(&e))
        {
            if(e.type == SDL_EVENT_QUIT)
                return 0;
        }
        

        SDL_SetRenderDrawColor(ren, r, g, b,255);
        SDL_RenderPoint(ren, to_screen_x(x), to_screen_y(y));
        SDL_RenderPresent(ren);
        
        if(r!=0 && b == 0)
        {
            r--;
            g++;
        }
        else if(g!=0)
        {
            g--;
            b++;
        }
        else if(b!=0)
        {
            b--;
            r++;
        }
        
        if(j<it)
        {
            x+=xdir;
            y+=ydir;
            j++;
        }
        else
        {
            j=0;
            if(xdir == -1 && ydir == -1) { xdir = 1; ydir = -1; }
            else if(xdir == 1 && ydir == -1) { xdir = 1; ydir = 1; }
            else if(xdir == 1 && ydir == 1) { xdir = -1; ydir = 1; }
            else if(xdir == -1 && ydir == 1) { xdir = -1; ydir = -1; it++; y++; }
        }
    }
    return 0;
}