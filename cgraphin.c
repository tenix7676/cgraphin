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
    
    while(true)
    {  
        while(SDL_PollEvent(&e))
        {
            if(e.type == SDL_EVENT_QUIT)
                return 0;
        }
        
        SDL_SetRenderDrawColor(ren, 0,0,0,50);
        SDL_RenderClear(ren);
        SDL_SetRenderDrawColor(ren, c,128-c,0,50);
        SDL_RenderPoint(ren, to_screen_x(x), to_screen_y(y));
        int ms=1;
        for(int it=0; it < 500; ++it)
        {
            for(int j=0; j < it; ++j)
            {
                x--;
                y--;
                SDL_SetRenderDrawColor(ren, c,255-c,0,255);
                SDL_RenderPoint(ren, to_screen_x(x), to_screen_y(y));
                SDL_RenderPresent(ren);
                c+=1;
                SDL_Delay(ms);
            }
            for(int j=0; j < it; ++j)
            {
                x++;
                y--;
                SDL_SetRenderDrawColor(ren, c,255-c,0,255);
                SDL_RenderPoint(ren, to_screen_x(x), to_screen_y(y));
                SDL_RenderPresent(ren);
                c+=1;
                SDL_Delay(ms);
            }
            for(int j=0; j < it; ++j)
            {
                x++;
                y++;
                SDL_SetRenderDrawColor(ren, c,255-c,0,255);
                SDL_RenderPoint(ren, to_screen_x(x), to_screen_y(y));
                SDL_RenderPresent(ren);
                c+=1;
                SDL_Delay(ms);
            }
            for(int j=0; j < it; ++j)
            {
                x--;
                y++;
                SDL_SetRenderDrawColor(ren, c,255-c,0,255);
                SDL_RenderPoint(ren, to_screen_x(x), to_screen_y(y));
                SDL_RenderPresent(ren);
                c+=1;
                SDL_Delay(ms);
            }
            y++;
        }
        SDL_RenderPresent(ren);
        x=0,y=0;
    }
    return 0;
}