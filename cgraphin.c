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
float min3(float a, float b, float c) {
    return fmin(fmin(a, b), c);
}

void hueToRGB(float h, float* r, float* g, float* b) {
    float kr = fmod(5+h*6, 6);
    float kg = fmod(3+h*6, 6);
    float kb = fmod(1+h*6, 6);

    *r = 1 - fmax(min3(kr, 4-kr, 1), 0);
    *g = 1 - fmax(min3(kg, 4-kg, 1), 0);
    *b = 1 - fmax(min3(kb, 4-kb, 1), 0);
}
enum shape
{
    SPIRAL,
    LEFT_TO_RIGHT,
    MAX_SHAPES
};
int main(int argc, char* argv[])
{
    SDL_Window* win;
    SDL_Renderer* ren;
    SDL_CreateWindowAndRenderer("cgraphin", 500, 500,SDL_WINDOW_TRANSPARENT | SDL_WINDOW_FULLSCREEN, &win, &ren);
    SDL_GetWindowSize(win, &width, &height);
    SDL_Event e;
    float hue=0;
    float r,g,b;
    int x,y;
    enum shape shp=LEFT_TO_RIGHT;
    switch(shp)
    {
    case SPIRAL:
        x=0; y=0;
        break;
    case LEFT_TO_RIGHT:
        x=-width/2; y=height/2;
        break;
    }
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
    while(true)
    {
        bool go=true;
        switch(shp)
        {
        case SPIRAL:
            go=it < max(width, height);
            break;
        case LEFT_TO_RIGHT:
            go=x < width/2;
            break;
        }
        if(go)
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
            hueToRGB(hue,&r,&g,&b);
            SDL_SetRenderDrawColorFloat(ren,r,g,b,1);
            SDL_RenderPoint(ren, to_screen_x(x), to_screen_y(y));
            SDL_RenderPresent(ren);
            if(x < -width/4)
                hue += 0.001;
            else if(x < 0)
                hue += 0.0001;
            else if(x < width/4)
                hue += 0.0001;
            else
                hue += 0.001;
        }
        }
        
        switch(shp)
        {
        case SPIRAL:
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
            break;
        case LEFT_TO_RIGHT:
            y--;
            if(y <= -height/2)
            {    
                x++;
                y=height/2;
            }
            break;
        }

    }
    return 0;
}