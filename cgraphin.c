#include <stdio.h>
#include <./SDL3/SDL.h>
#include <./SDL3/SDL_main.h>

/*compile with
cl.exe /I./ cgraphin.c /link /defaultlib:sdl3 /subsystem:console && cgraphin.exe
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
    SDL_SetRenderDrawColor(ren, 0,0,255,1);
    SDL_RenderClear(ren);
    
    
    bool dec_green=false,dec_blue=false;
    while(true)
    {  
        while(SDL_PollEvent(&e))
        {
            if(e.type == SDL_EVENT_QUIT)
                return 0;
        }
        
        if(r!=0)
        {
        g=255-r;
        b=0;
        SDL_SetRenderDrawColor(ren, r, g, b,255);
        SDL_RenderPoint(ren, to_screen_x(x+g-(255-r)/2), to_screen_y(y));
        while(g!=0)
        {
            g--;
            b++;
            SDL_SetRenderDrawColor(ren, r, g, b,255);
            SDL_RenderPoint(ren, to_screen_x(x+g-(255-r)/2), to_screen_y(y));
        }
        r--;
        y--;
        
        SDL_RenderPresent(ren);
        SDL_Delay(5);
        }
    }
    return 0;
}