#include <stdio.h>
#include <stdlib.h>
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
int to_graph_x(int scr_x)
{
    return scr_x-width/2;
}
int to_graph_y(int scr_y)
{
    return -(scr_y-height/2);
}
double f(double x)
{
    if(x==0) return 0;
    return sin(1/x);
}

char** map;

double scale=1./512;
void graph_function_to_map()
{
    for(size_t i=0; i < height; ++i)
        for(size_t j=0; j < width; ++j)
        {
            int y = to_graph_y(i);
            int x = to_graph_x(j);
            double s=scale*y-f(scale*x);
            char sign=(s>0)-(s<0);
            map[i][j]=sign;
        }
}

int main(int argc, char* argv[])
{
    SDL_Window* win;
    SDL_Renderer* ren;
    SDL_CreateWindowAndRenderer("cgraphin", 500, 500,SDL_WINDOW_TRANSPARENT | SDL_WINDOW_FULLSCREEN, &win, &ren);
    SDL_GetWindowSize(win, &width, &height);
    SDL_Event e;
    int x=0,y=0;
    int it=0, j=0;
    int xdir=-1,ydir=1;
    map = (char**)malloc(height*sizeof(char*));
    for(size_t i=0; i < height; ++i)
    {
        map[i]=(char*)malloc(width*sizeof(char));
    }
    graph_function_to_map();
    
    SDL_SetRenderDrawColor(ren, 0,0,0,0);
    SDL_RenderClear(ren);
    SDL_SetRenderDrawColor(ren, 255,255,255,255);
    while(true)
    {  
        if(it < fmax(width, height))
            while(SDL_PollEvent(&e) != 0)
            {
                if(e.type == SDL_EVENT_QUIT)
                    return 0;
            }
        else
            while(SDL_WaitEvent(&e) != 0)
            {
                if(e.type == SDL_EVENT_QUIT)
                    return 0;
            }
        if(-width/ 2 < x && x < width / 2 - 1 && -height/ 2 < y && y < height / 2 - 1)
        {
        char s1=map[to_screen_y(y+0)][to_screen_x(x+0)];
        char s2=map[to_screen_y(y+0)][to_screen_x(x+1)];
        char s3=map[to_screen_y(y+1)][to_screen_x(x+0)];
        char s4=map[to_screen_y(y+1)][to_screen_x(x+1)];
        if(!((s1<0&&s2<0&&s3<0&&s4<0)
          || (s1>0&&s2>0&&s3>0&&s4>0)) )
        {
            SDL_RenderPoint(ren, to_screen_x(x), to_screen_y(y));
            SDL_RenderPresent(ren);
        }
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