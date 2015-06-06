#include "color.h"
#include "closepointgenerator.h"
#include "closepointinterpolategenerator.h"
#include "closepointdithergenerator.h"
#include "xpmgenerator.h"
#include "ppmgenerator.h"
#include <cstdio>
#include <cstdlib>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>
#include "vec4.h"
//#define DEBUG
int main (int argc, char **argv)
{
    #ifdef DEBUG
    int xSize,ySize;
         struct winsize w;
            ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
            xSize=w.ws_col;
            ySize=w.ws_row;
            Ascii gfx(xSize,ySize);
        //ClosePointInterpolateGenerator generator=ClosePointInterpolateGenerator();
        PPMGenerator generator;
        gfx.doSomething();
        gfx.bitblit(&generator);
        return 0;
    #endif
    if(argc!=3&&argc!=5){
        printf("%s objFile format xSize ySize\nformat can be ppm,xpm,console,dither\n",argv[0]);
    }else{
        int xSize,ySize;
        if(argc==5){
            xSize=atoi(argv[3]);
            ySize=atoi(argv[4]);
        }else{
            struct winsize w;
            ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
            xSize=w.ws_col;
            ySize=w.ws_row;
        }
        Ascii gfx(xSize,ySize,argv[1]);
        gfx.doSomething();
        if(std::string(argv[2]).compare("console")==0){
            ClosePointInterpolateGenerator generator;
            while(true)gfx.bitblit(&generator);
        }else if(std::string(argv[2]).compare("dither")==0){
            ClosePointDitherGenerator generator;
            while(true)gfx.bitblit(&generator);
        }else if(std::string(argv[2]).compare("ppm")==0){
            PPMGenerator generator;
            gfx.bitblit(&generator);
        }else if(std::string(argv[2]).compare("xpm")==0){
            XPMGenerator generator;
            gfx.bitblit(&generator);
        }else{
            printf("bad format\n");
            return 0;
        }
    }
    return 0;
}
