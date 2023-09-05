#include "color.h"
#include "colorgenerator.h"
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include "closepointinterpolategenerator.h"
#include "mat4.h"

Ascii::Ascii(int sizeX,int sizeY,const char* model){
    this->sizeX=sizeX;
    this->sizeY=sizeY;
    noDepth=false;
    initMap();
    render=new Render(sizeX,sizeY);
    obj=OBJMesh(model);
}

Ascii::~Ascii(){
    for(int i=0;i<sizeX;i++){
        delete[] bitmap[i];
        delete[] depth[i];
    }
    delete[] bitmap;
    delete[] depth;
    delete render;
}

void Ascii::initMap(){
    bitmap=new ColorRaw*[sizeX];
    depth=new float*[sizeX];
    for(int i=0;i<sizeX;i++){
        bitmap[i]=new ColorRaw[sizeY];
        depth[i]=new float[sizeY];
    }

    for(int i=0;i<sizeX;i++)
    for(int j=0;j<sizeY;j++){
        bitmap[i][j].r=bitmap[i][j].g=bitmap[i][j].b;
        depth[i][j]=1.0;
    }
}

void Ascii::bitblit(ColorGenerator* generator){
    static int t=180;
    doSomething(t++);
    generator->output(sizeX,sizeY,bitmap);
}

double Ascii::func(int i,int j,int t){
    int x=cos(t*0.01)*sizeX/2+sizeX/2;
    int y=sin(t*0.01)*sizeX/2+sizeY/2;
    if(x<0)x=0;
    if(x>sizeX)x=sizeX;
    if(y<0)y=0;
    if(y>sizeY)y=sizeY;
    double c=0;
    c+=cos(t*0.01+sqrt((sizeX/2-i)*(sizeX/2-i)+(sizeY/2-j)*(sizeY/2-j))*0.5);
    c+=cos(-t*0.1+sqrt((x-i)*(x-i)+(y-j)*(y-j))*0.5);
    c*=0.5;
    c+=0.5;
    return c<0?0:c>1?1:c;
}
double Ascii::func2(int i,int j,int t,double n){
    int x1=cos(t*(0.01+n))    *sizeY/3+sizeX/2;
    int y1=sin(t*(0.02+n*1.2))*sizeY/3+sizeY/2;
    int x2=cos(t*(0.01+n*1.1))*sizeY/3+sizeX/2;
    int y2=sin(t*(0.02+n*0.9))*sizeY/3+sizeY/2;
    double c=0;
    double d1=sizeX/6-sqrt((x1-i)*(x1-i)+(y1-j)*(y1-j));
    double d2=sizeX/6-sqrt((x2-i)*(x2-i)+(y2-j)*(y2-j));
    d1/=sizeX/8.0;
    d2/=sizeX/8.0;
    c+=d1<0?0:d1;
    c+=d2<0?0:d2;
    return c<0?0:c>1?1:c;
}
void Ascii::doSomething(int t){
    clearBitmap();
    clearDepth();
    std::vector<vec4> data=obj.getData();
    vec4 c[]={vec4(1.0,0.0,0.0,0.0),
              vec4(0.0,1.0,0.0,0.0),
              vec4(0.0,0.0,1.0,0.0)};

    //float r=(float)sizeX/sizeY;//ratio
    const float char_ratio = 17.0/8.0;//char aspect ratio 
    mat4 mn = mat4::rotation(1,0,0,t)*mat4::rotation(0,1,0,t/2.0-90.0);
    mat4 m=mat4::translate(sizeX/2.0,sizeY/2.0,-0.1)*mat4::scale(sizeY*0.8,sizeY*0.8/char_ratio,1.0)*mn;
    for(int i=0;i<data.size()/6;i++){
        vec4 p[3];
        vec4 n[3];
        //mat4 proj=mat4::persp(-1,1,-1,1,0,3);
        for(int j=0;j<3;j++){
            p[j]=m*data.data()[i*6+j];
            n[j]=mn*data.data()[i*6+j+3];
        }

        putTripleFill(p,c,n);
    }
}

void Ascii::doPlasma(){
    int t=100;
    for(int j=0;j<this->sizeY;j++)
    for(int i=0;i<this->sizeX;i++){
    /*
        putColor(i,j,
                 func2(i,j,t,0.01),
                 func2(i,j,t*2,0.06),
                 func2(i,j,t*3,0.03));
    //*/
        putColor(vec4(i,j,0.0,0.0),
                 vec4(func(i,j,t),
                      func(i,j,t*2),
                      func(i,j,t*3),
                      0.0));
    }
}

void Ascii::putColorZ(vec4 position,vec4 color){
    if(noDepth){
        putColor(position,color);
        return;
    }
    int x=position.x;
    int y=position.y;
    if(depth[x][y]>position.z){
       depth[x][y]=position.z;
       putColor(position,color);
    }
}

void Ascii::putColor(vec4 position, vec4 color){
    if(position.x<0||position.y<0||position.x>=sizeX||position.y>=sizeY)return;
    int x=position.x;
    int y=position.y;
    color=color.clamp();
    bitmap[x][y].r=color.x*255;
    bitmap[x][y].g=color.y*255;
    bitmap[x][y].b=color.z*255;
}

void Ascii::putLine(vec4* position, vec4* color,int ra,int rb){
    vec4 a=position[ra];
    vec4 b=position[rb];
    vec4 v=b-a;
    if(abs(v.x)>=abs(v.y)){
        vec4 aa=a.x<=b.x?a:b;
        vec4 bb=a.x> b.x?a:b;
        vec4 ca=a.x<=b.x?color[ra]:color[rb];
        vec4 cb=a.x> b.x?color[ra]:color[rb];
        if(abs(v.x)>abs(v.y))putLineX(aa,bb,ca,cb);
        else putLineD(aa,bb,ca,cb);
    }else{
        vec4 aa=a.y<=b.y?a:b;
        vec4 bb=a.y> b.y?a:b;
        vec4 ca=a.y<=b.y?color[ra]:color[rb];
        vec4 cb=a.y> b.y?color[ra]:color[rb];
        putLineY(aa,bb,ca,cb);
    }
}
void Ascii::putTriple(vec4* position, vec4* color){
    putLine(position,color,0,1);
    putLine(position,color,0,2);
    putLine(position,color,1,2);
}
void Ascii::putTripleFill(vec4* position, vec4* color, vec4* normal){
    render->stencilReset();
    render->workTriple(position,color,normal);
    while(!render->done()){
        vec4 p,c;
        render->getPixel(p,c);
        putColorZ(p,c);
    }
}
void Ascii::putLineX(vec4 positionA,vec4 positionB,vec4 colorA,vec4 colorB){
    vec4 a=positionA.pixelize();
    vec4 b=positionB.pixelize();
    for(float x=a.x;x<=b.x;x++){
        float p=(x-a.x)/(b.x-a.x);
        float yi=((b-a)*p).y;
        float y0=yi-floor(yi);
        float y1=yi-ceil(yi);
        float y=(y0<=y1?floor(yi):ceil(yi))+floor(a.y);
        float z=((b-a)*p+a).z;
        printf("\n%f\n",z);
        /*float dy0=floor(y)-y;
        float dy1=ceil(y)-y;
        y=dy0<dy1?floor(y):ceil(y);*/
        vec4 v=vec4(x,y,z,0.0);
        vec4 c=(colorB-colorA)*p+colorA;
        putColorZ(v,c);
    }
}
void Ascii::putLineY(vec4 positionA,vec4 positionB,vec4 colorA,vec4 colorB){
    vec4 a=positionA.pixelize();
    vec4 b=positionB.pixelize();
    for(float y=a.y;y<=b.y;y++){
        float p=(y-a.y)/(b.y-a.y);
        float xi=((b-a)*p).x;
        float x0=xi-floor(xi);
        float x1=xi-ceil(xi);
        float x=(x0<=x1?floor(xi):ceil(xi))+floor(a.x);
        float z=((b-a)*p+a).z;
        printf("\n%f\n",z);
        /*float dx0=floor(x)-x;
        float dx1=ceil(x)-x;
        x=dx0<dx1?floor(x):ceil(x);*/
        vec4 v=vec4(x,y,z,0.0);
        vec4 c=(colorB-colorA)*p+colorA;
        putColorZ(v,c);
    }
}
void Ascii::putLineD(vec4 positionA,vec4 positionB,vec4 colorA,vec4 colorB){
    vec4 a=positionA.pixelize();
    vec4 b=positionB.pixelize();
    for(float x=a.x;x<=b.x;x++){
        float p=(x-a.x)/(b.x-a.x);
        float y=(x-a.x)+a.y;
        float z=((b-a)*p+a).z;
        printf("\n%f-%f\n",z,x);
        /*float dx0=floor(x)-x;
        float dx1=ceil(x)-x;
        x=dx0<dx1?floor(x):ceil(x);*/
        vec4 v=vec4(x,y,z,0.0);
        vec4 c=(colorB-colorA)*p+colorA;
        putColorZ(v,c);
    }
}

void Ascii::clearDepth(){
    for(int j=0;j<sizeY;j++)
    for(int i=0;i<sizeX;i++)
        depth[i][j]=1.0;
}
void Ascii::clearBitmap(){
    for(int j=0;j<sizeY;j++)
    for(int i=0;i<sizeX;i++)
        bitmap[i][j].r=bitmap[i][j].g=bitmap[i][j].b=0.0;
}
