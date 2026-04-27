#include <stdio.h>
#include <stdlib.h>
#include <./SDL3/SDL.h>
#include <./SDL3/SDL_main.h>

double f(double);
//compile with
/*
cl.exe /I./ cgraphin.c /link /defaultlib:sdl3 /subsystem:console && cgraphin.exe
*/
typedef struct Lab {float L; float a; float b;} Lab;
typedef struct RGB {float r; float g; float b;} RGB;

Lab linear_srgb_to_oklab(RGB c) 
{
    float l = 0.4122214708f * c.r + 0.5363325363f * c.g + 0.0514459929f * c.b;
	float m = 0.2119034982f * c.r + 0.6806995451f * c.g + 0.1073969566f * c.b;
	float s = 0.0883024619f * c.r + 0.2817188376f * c.g + 0.6299787005f * c.b;

    float l_ = cbrtf(l);
    float m_ = cbrtf(m);
    float s_ = cbrtf(s);

    Lab lab={
        0.2104542553f*l_ + 0.7936177850f*m_ - 0.0040720468f*s_,
        1.9779984951f*l_ - 2.4285922050f*m_ + 0.4505937099f*s_,
        0.0259040371f*l_ + 0.7827717662f*m_ - 0.8086757660f*s_,
    };
    return lab;
}

RGB oklab_to_linear_srgb(Lab c) 
{
    float l_ = c.L + 0.3963377774f * c.a + 0.2158037573f * c.b;
    float m_ = c.L - 0.1055613458f * c.a - 0.0638541728f * c.b;
    float s_ = c.L - 0.0894841775f * c.a - 1.2914855480f * c.b;

    float l = l_*l_*l_;
    float m = m_*m_*m_;
    float s = s_*s_*s_;

    RGB rgb={
		+4.0767416621f * l - 3.3077115913f * m + 0.2309699292f * s,
		-1.2684380046f * l + 2.6097574011f * m - 0.3413193965f * s,
		-0.0041960863f * l - 0.7034186147f * m + 1.7076147010f * s,
    };
    return rgb;
}

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

char** map;

double scale;
typedef struct thread_data
{
    char** map;
    size_t row_start;
    size_t row_end;
    size_t col_start;
    size_t col_end;
} thread_data;
//func has to look like this:
// typedef int (SDLCALL *SDL_ThreadFunction) (void *data);
int thread_func(void* data)
{
    thread_data td= *((thread_data*)data);
    printf("td: %p\n%d\n%d\n%d\n%d\n", td.map,td.row_start,td.row_end,td.col_start,td.col_end);
    for(size_t i=td.row_start; i < td.row_end; ++i)
        for(size_t j=td.col_start; j < td.col_end; ++j)
        {
            int y = to_graph_y(i);
            int x = to_graph_x(j);
            double s=scale*y-f(scale*x);
            char sign=(s>0)-(s<0);
            td.map[i][j]=sign;
        }
    return 0;
}
void graph_function_to_map()
{
    //lets try:
    //4  threads done
    //12 threads
    //n  threads?
    SDL_Thread* threads[4];
    thread_data tds[4];
    
    
    
    tds[0].map=map;
    tds[0].row_start=0;
    tds[0].row_end=height/2;
    tds[0].col_start=0;
    tds[0].col_end=width/2;
    threads[0]=SDL_CreateThread(thread_func,"0",&tds[0]);

    tds[1].map=map;
    tds[1].row_start=0;
    tds[1].row_end=height/2;
    tds[1].col_start=width/2;
    tds[1].col_end=width;
    threads[1]=SDL_CreateThread(thread_func,"1",&tds[1]);
    
    tds[2].map=map;
    tds[2].row_start=height/2;
    tds[2].row_end=height;
    tds[2].col_start=0;
    tds[2].col_end=width/2;
    threads[2]=SDL_CreateThread(thread_func,"2",&tds[2]);

    tds[3].map=map;
    tds[3].row_start=height/2;
    tds[3].row_end=height;
    tds[3].col_start=width/2;
    tds[3].col_end=width;
    threads[3]=SDL_CreateThread(thread_func,"3",&tds[3]);
    
    int status=0;
    for(size_t i=0; i<4; ++i)
    {
        SDL_WaitThread(threads[i], &status);
        printf("%p status: %d\n", threads[i], status);
    }
    // for(size_t i=0; i < height; ++i)
        // for(size_t j=0; j < width; ++j)
        // {
            // int y = to_graph_y(i);
            // int x = to_graph_x(j);
            // double s=scale*y-f(scale*x);
            // char sign=(s>0)-(s<0);
            // map[i][j]=sign;
        // }
}

enum shape
{
    SPIRAL,
    LEFT_TO_RIGHT,
    MAX_SHAPES
};
double scale=1./128;
double f(double x)
{
    // return x;
    double result=sin(x);
    for(int i=0; i < 1000; ++i)
        result = sin(tan(result));
    return result;
}
int main(int argc, char* argv[])
{
    SDL_Window* win;
    SDL_Renderer* ren;
    SDL_CreateWindowAndRenderer("cgraphin", 500, 500,SDL_WINDOW_TRANSPARENT | SDL_WINDOW_FULLSCREEN, &win, &ren);
    SDL_GetWindowSize(win, &width, &height);
    SDL_Event e;
    float L=.9;
    float C=0.125;
    float h=0.0;
    int thick_x=2;
    int thick_y=2;
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
            Lab lab = {L,C*cos(h),C*sin(h)};
            RGB rgb = oklab_to_linear_srgb(lab);
            SDL_SetRenderDrawColorFloat(ren,rgb.r,rgb.g,rgb.b,1);
            SDL_FRect rect={ to_screen_x(x-thick_x/2), to_screen_y(y-thick_y/2),thick_x/2, thick_y/2 };
            SDL_RenderFillRect(ren,&rect);
            SDL_RenderPresent(ren);
            h += 0.005;
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