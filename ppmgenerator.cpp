#include "ppmgenerator.h"
#include <cstdio>

PPMGenerator::PPMGenerator()
             :ColorGenerator(){

}

PPMGenerator::~PPMGenerator(){

}

void PPMGenerator::output(int x,int y,ColorRaw**bitmap){
    printf("P3\n");
    printf("%i %i\n255\n",x,y);
    for(int j=0;j<y;j++){
    for(int i=0;i<x;i++)
        printf("%i %i %i ",
               (unsigned char)bitmap[i][j].r,
               (unsigned char)bitmap[i][j].g,
               (unsigned char)bitmap[i][j].b);
        printf("\n");
    }
}
