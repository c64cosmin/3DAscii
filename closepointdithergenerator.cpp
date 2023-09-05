#include"closepointdithergenerator.h"
#include<cmath>
#include<cstdio>

ClosePointDitherGenerator::ClosePointDitherGenerator()
                          :ColorGenerator(){
    generateColors();
}

ClosePointDitherGenerator::~ClosePointDitherGenerator(){

}

void ClosePointDitherGenerator::generateColors(){
    diether[0]=" ";
    diether[1]="/";
    diether[2]="%";
    diether[3]="#";
    diether[0]=" ";
    diether[1]="\u2591";
    diether[2]="\u2592";
    diether[3]="\u2593";//*/
    initColorPoints();
}

ColorData ClosePointDitherGenerator::interpolate(double x,int color_a,int color_b){
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

void ClosePointDitherGenerator::addColorPoint(double r,double g,double b,int color){
    rCoord.push_back(r);
    gCoord.push_back(g);
    bCoord.push_back(b);
    cCoord.push_back(color);
}

void ClosePointDitherGenerator::initColorPoints(){
//init dither map
    ditherMap[0][0]= 1;ditherMap[1][0]= 9;ditherMap[2][0]= 3;ditherMap[3][0]=11;
    ditherMap[0][1]=13;ditherMap[1][1]= 5;ditherMap[2][1]=15;ditherMap[3][1]= 7;
    ditherMap[0][2]= 4;ditherMap[1][2]=12;ditherMap[2][2]= 2;ditherMap[3][2]=10;
    ditherMap[0][3]=16;ditherMap[1][3]= 8;ditherMap[2][3]=14;ditherMap[3][3]= 6;
    for(int i=0;i<4;i++)
    for(int j=0;j<4;j++)
        ditherMap[i][j]/=17.0;

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

ColorData ClosePointDitherGenerator::getGuess(double r,double g,double b){
    double d=10;
    int n=rCoord.size();
    int m1=0,m2=0;
    for(int i=0;i<n;i++){
		double rd = r-rCoord[i];
		double gd = g-gCoord[i];
		double bd = b-bCoord[i];
        double dist=sqrt(rd*rd+gd*gd+bd*bd);
        if(dist<=d){
            m1=i;
            d=dist;
        }
    }
    //can make this faster
    d=10;
    for(int i=0;i<n;i++){
		double rd = r-rCoord[i];
		double gd = g-gCoord[i];
		double bd = b-bCoord[i];
        double dist=sqrt(rd*rd+gd*gd+bd*bd);
        if(dist<=d&&i!=m1){
            m2=i;
            d=dist;
        }
    }
	double rm1 = rCoord[m1];
	double gm1 = gCoord[m1];
	double bm1 = bCoord[m1];
	double rm2 = rCoord[m2];
	double gm2 = gCoord[m2];
	double bm2 = bCoord[m2];
    double dm1=sqrt(rm1*rm1+gm1*gm1+bm1*bm1);
    double dm2=sqrt(rm2*rm2+gm2*gm2+bm2*bm2);
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

void ClosePointDitherGenerator::putColor(int x,int y,double r,double g,double b){
    double oldR=r+(r*ditherMap[x%4][y%4]);
    double oldG=g+(g*ditherMap[x%4][y%4]);
    double oldB=b+(b*ditherMap[x%4][y%4]);
    ColorData c=getGuess(oldR,oldG,oldB);
    printf("\033[%i;%im%s",c.fg,
                           c.bg,
                           diether[(int)c.diether]);
}

void ClosePointDitherGenerator::output(int x,int y,ColorRaw**bitmap){
    printf("\033[0;0H");
    for(int j=0;j<y;j++)
    for(int i=0;i<x;i++)
        putColor(i,j,
                 bitmap[i][j].r/255.0,
                 bitmap[i][j].g/255.0,
                 bitmap[i][j].b/255.0);
}

