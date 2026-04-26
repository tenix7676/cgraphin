#include <stdio.h>
#include <./SDL3/SDL.h>
#include <./SDL3/SDL_main.h>

//compile with
/*
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

double f(double x)
{
    return x*x;
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
    
    double step = 1;
    
    SDL_SetRenderDrawColor(ren, 0,0,0,50);
    SDL_RenderClear(ren);
    while(true)
    {  
        while(SDL_PollEvent(&e))
        {
            if(e.type == SDL_EVENT_QUIT)
                return 0;
        }
        
        double s1 = (y-f(x));
        double s2 = (y-f(x-step));
        double s3 = (y+step-f(x));
        double s4 = (y+step-f(x-step));
        if(!((s1<0&&s2<0&&s3<0&&s4<0)
          || (s1>0&&s2>0&&s3>0&&s4>0)) )
        {
        SDL_SetRenderDrawColor(ren, 255,255,255,255);            
        }
        else
        {
        SDL_SetRenderDrawColor(ren, 0,0,0,0);            
        }
        SDL_RenderPoint(ren, to_screen_x(x), to_screen_y(y));
        SDL_RenderPresent(ren);
        
        
        
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