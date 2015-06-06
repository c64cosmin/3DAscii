#include"closepointinterpolategenerator.h"
#include<cmath>
#include<cstdio>

ClosePointInterpolateGenerator::ClosePointInterpolateGenerator()
                               :ColorGenerator(){
    generateColors();
}

ClosePointInterpolateGenerator::~ClosePointInterpolateGenerator(){

}

void ClosePointInterpolateGenerator::generateColors(){
    diether[0]=" ";
    diether[1]="/";
    diether[2]="%";
    diether[3]="#";
    diether[0]=" ";
    diether[1]="\u2591";
    diether[2]="\u2592";
    diether[3]="\u2593";//*/
    initColorPoints();
    for(int r=0;r<GUESS_RESOLUTION;r++)
    for(int g=0;g<GUESS_RESOLUTION;g++)
    for(int b=0;b<GUESS_RESOLUTION;b++){
        colordata[r][g][b]=getGuess(r/(GUESS_RESOLUTION-1.0),
                                    g/(GUESS_RESOLUTION-1.0),
                                    b/(GUESS_RESOLUTION-1.0));
    }
}

ColorData ClosePointInterpolateGenerator::interpolate(double x,int color_a,int color_b){
    ColorData dummy;
    dummy.bg=(color_a&PLAIN_COLOR)+((color_a&BRIGHT)==BRIGHT?100:40);
    dummy.fg=(color_b&PLAIN_COLOR)+((color_b&BRIGHT)==BRIGHT? 90:30);
    if(x<0)x=0;
    if(x>1)x=1;
    int i=floor(x*4.0+0.5);
    if(i<0)i=0;
    if(i>4)i=4;
    dummy.diether=i;
    if(i==4){
        dummy.bg=(color_b&PLAIN_COLOR)+((color_b&BRIGHT)==BRIGHT?100:40);
        dummy.fg=30;
        dummy.diether=0;
    }
    return dummy;
}

void ClosePointInterpolateGenerator::addColorPoint(double r,double g,double b,int color){
    rCoord.push_back(r);
    gCoord.push_back(g);
    bCoord.push_back(b);
    cCoord.push_back(color);
}

void ClosePointInterpolateGenerator::initColorPoints(){
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

ColorData ClosePointInterpolateGenerator::getGuess(double r,double g,double b){
    double d=10;
    int n=rCoord.size();
    int m1=0,m2=0;
    for(int i=0;i<n;i++){
        double dist=sqrt((r-rCoord[i])*(r-rCoord[i])+
                         (g-gCoord[i])*(g-gCoord[i])+
                         (b-bCoord[i])*(b-bCoord[i]));
        if(dist<=d){
            m1=i;
            d=dist;
        }
    }
    //can make this faster
    d=10;
    for(int i=0;i<n;i++){
        double dist=sqrt((r-rCoord[i])*(r-rCoord[i])+
                         (g-gCoord[i])*(g-gCoord[i])+
                         (b-bCoord[i])*(b-bCoord[i]));
        if(dist<=d&&i!=m1){
            m2=i;
            d=dist;
        }
    }
    double dm1=sqrt(rCoord[m1]*rCoord[m1]+
                    gCoord[m1]*gCoord[m1]+
                    bCoord[m1]*bCoord[m1]);
    double dm2=sqrt(rCoord[m2]*rCoord[m2]+
                    gCoord[m2]*gCoord[m2]+
                    bCoord[m2]*bCoord[m2]);
    if(dm2<dm1){
        double aux=m1;
        m1=m2;
        m2=aux;
    }
    //vec A and B
    double Ar=r-rCoord[m1];
    double Ag=g-gCoord[m1];
    double Ab=b-bCoord[m1];
    double Br=rCoord[m2]-rCoord[m1];
    double Bg=gCoord[m2]-gCoord[m1];
    double Bb=bCoord[m2]-bCoord[m1];
    //norm
    //double dA=sqrt(Ar*Ar+Ag*Ag+Ab*Ab);
    double dB=sqrt(Br*Br+Bg*Bg+Bb*Bb);
    //normalize
    /*
    Ar/=dA;
    Ag/=dA;
    Ab/=dA;//*/

    Br/=dB;
    Bg/=dB;
    Bb/=dB;//*/
    //dot product;
    double dot=Ar*Br+Ag*Bg+Ab*Bb;
    dot/=dB;

    return interpolate(dot,cCoord[m1],cCoord[m2]);
}

void ClosePointInterpolateGenerator::putColor(double r,double g,double b){
    int rGuess=(int)(r*GUESS_RESOLUTION);
    int gGuess=(int)(g*GUESS_RESOLUTION);
    int bGuess=(int)(b*GUESS_RESOLUTION);
    if(rGuess==GUESS_RESOLUTION)rGuess--;
    if(gGuess==GUESS_RESOLUTION)gGuess--;
    if(bGuess==GUESS_RESOLUTION)bGuess--;
    ColorData c=colordata[rGuess][gGuess][bGuess];//*/
    //ColorData c=getGuess(r,g,b);
    printf("\033[%i;%im%s",c.fg,
                           c.bg,
                           diether[(int)c.diether]);
}

void ClosePointInterpolateGenerator::output(int x,int y,ColorRaw**bitmap){
    printf("\033[0;0H");
    for(int j=0;j<y;j++)
    for(int i=0;i<x;i++)
        putColor(bitmap[i][j].r/255.0,
                 bitmap[i][j].g/255.0,
                 bitmap[i][j].b/255.0);
}
