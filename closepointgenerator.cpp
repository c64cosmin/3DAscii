#include"closepointgenerator.h"
#include<cmath>
#include<cstdio>

ClosePointGenerator::ClosePointGenerator()
                    :ColorGenerator(){
    generateColors();
}

ClosePointGenerator::~ClosePointGenerator(){

}

void ClosePointGenerator::generateColors(){
    diether[0]=" ";
    diether[1]="/";
    diether[2]="%";
    diether[3]="#";
    diether[0]=" ";
    diether[1]="\u2591";
    diether[2]="\u2592";
    diether[3]="\u2593";
    initColorPoints();
    for(int r=0;r<16;r++)
    for(int g=0;g<16;g++)
    for(int b=0;b<16;b++){
        colordata[r][g][b]=getGuess(r/15.0,g/15.0,b/15.0);
    }
}

ColorData ClosePointGenerator::convert(int color){
    ColorData dummy;
    dummy.bg=(color&PLAIN_COLOR)+((color&BRIGHT)==BRIGHT?100:40);
    dummy.fg=30;
    dummy.diether=0;
    return dummy;
}

void ClosePointGenerator::addColorPoint(double r,double g,double b,int color){
    rCoord.push_back(r);
    gCoord.push_back(g);
    bCoord.push_back(b);
    cCoord.push_back(color);
}

void ClosePointGenerator::initColorPoints(){
    addColorPoint(0  ,0  ,0  ,0);
    addColorPoint(0.5,0  ,0  ,RED);
    addColorPoint(0  ,0.5,0  ,GREEN);
    addColorPoint(0  ,0  ,0.5,BLUE);
    addColorPoint(0.5,0  ,0.5,RED+BLUE);
    addColorPoint(0.5,0.5,0  ,RED+GREEN);
    addColorPoint(0  ,0.5,0.5,GREEN+BLUE);
    addColorPoint(0.666,0.666,0.666,RED+GREEN+BLUE);
    addColorPoint(0.333,0.333,0.333,BRIGHT);
    addColorPoint(1.0,0  ,0  ,RED+BRIGHT);
    addColorPoint(0  ,1.0,0  ,GREEN+BRIGHT);
    addColorPoint(0  ,0  ,1.0,BLUE+BRIGHT);
    addColorPoint(1.0,0  ,1.0,RED+BLUE+BRIGHT);
    addColorPoint(1.0,1.0,0  ,RED+GREEN+BRIGHT);
    addColorPoint(0  ,1.0,1.0,GREEN+BLUE+BRIGHT);
    addColorPoint(1.0,1.0,1.0,RED+GREEN+BLUE+BRIGHT);
}

ColorData ClosePointGenerator::getGuess(double r,double g,double b){
    double d=10;
    int n=rCoord.size();
    int m=0;
    for(int i=0;i<n;i++){
        double rd = r-rCoord[i];
        double gd = g-gCoord[i];
        double bd = b-bCoord[i];
        double dist=sqrt(rd*rd+gd*gd+bd*bd);
        if(dist<=d){
            m=i;
            d=dist;
        }
    }
    return convert(cCoord[m]);
}

void ClosePointGenerator::putColor(double r,double g,double b){
    int rr=r*15;
    int gg=g*15;
    int bb=b*15;
    rr=(rr>15?15:rr<0?0:rr);
    gg=(gg>15?15:gg<0?0:gg);
    bb=(bb>15?15:bb<0?0:bb);
    printf("\033[%i;%im%s",colordata[rr][gg][bb].fg,
                           colordata[rr][gg][bb].bg,
              diether[(int)colordata[rr][gg][bb].diether]);
}

void ClosePointGenerator::output(int x,int y,ColorRaw**bitmap){
    printf("\033[0;0H");
    for(int j=0;j<y;j++)
    for(int i=0;i<x;i++)
        putColor(bitmap[i][j].r/255.0,
                 bitmap[i][j].g/255.0,
                 bitmap[i][j].b/255.0);
}
