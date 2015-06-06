#include "render.h"
#include "color.h"
#include <cstdlib>
#include <cstdio>
#include "mat3.h"

Render::Render(int sizeX,int sizeY){
    this->sizeX=sizeX;
    this->sizeY=sizeY;
    stencil=new bool*[sizeX];
    for(int i=0;i<sizeX;i++)
        stencil[i]=new bool[sizeY];
}

Render::~Render(){
    for(int i=0;i<sizeX;i++)
        delete[] stencil[i];
    delete[] stencil;
}

void Render::workTriple(vec4* position,vec4* color,vec4* normal){
    workPosition=position;
    workColor=color;
    workNormal=normal;
    /*for(int i=0;i<100;i++){
        vec4 bound=getBound(position);
        vec4 p(bound.x+(bound.y-bound.x)*getFloat(),
               bound.z+(bound.w-bound.z)*getFloat(),
               0.0,
               0.0);
        p=p.pixelize();
        addPixel(p);
    }*/
    vec4 bound=getBound(position);
    for(int i=bound.x;i<=bound.y;i++)
    for(int j=bound.z;j<=bound.w;j++){
        vec4 p(i,j,0,0);
        addPixel(p.pixelize());
    }
}

void Render::getPixel(vec4 &p,vec4 &c){
    if(qPixels.empty()){
        p=vec4(-1.0,0.0,0.0,0.0);
        return;
    }
    vec4 pixel=qPixels.front();
    //addNeighbors(pixel);
    qPixels.pop();
    //shader
    p=pixel;
    p.z=getDepth(pixel);
    vec4 normal=getNormal(pixel);
    vec4 lightDirection=-vec4(1,1,1,0).normalize();
    float light=normal.normalize()^lightDirection;
    light*=0.8;
    float shine=pow(light+0.2,20.0);
    c=vec4(light*0.8+shine,
           light*0.2+shine,
           light*0.1+shine,1);

    lightDirection=vec4(1,1,-1,0).normalize();
    light=normal.normalize()^lightDirection;
    light*=0.3;
    shine=pow(light+0.1,20.0);
    c=c+vec4(light*0.1+shine,
             light*0.3+shine,
             light*0.6+shine,1);
}

bool Render::contains(vec4 v){
    if(!onTriangle(v))return true;
    int x=v.x;
    int y=v.y;
    if(x<0||y<0||x>=sizeX||y>=sizeY)return true;
    return stencil[x][y];
}
bool Render::onTriangle(vec4 v){

    mat3 m;

    m.mat[0][0]=workPosition[0].x;m.mat[1][0]=workPosition[0].y;m.mat[2][0]=1.0;
    m.mat[0][1]=workPosition[1].x;m.mat[1][1]=workPosition[1].y;m.mat[2][1]=1.0;
    m.mat[0][2]=workPosition[2].x;m.mat[1][2]=workPosition[2].y;m.mat[2][2]=1.0;
    bool flip=false;
    if(m.det()<0.0)flip=true;

    m.mat[0][0]=workPosition[0].x;m.mat[1][0]=workPosition[0].y;m.mat[2][0]=1.0;
    m.mat[0][1]=workPosition[1].x;m.mat[1][1]=workPosition[1].y;m.mat[2][1]=1.0;
    m.mat[0][2]=v.x;m.mat[1][2]=v.y;m.mat[2][2]=1.0;
    if(flip&&m.det()>0.0)return false;
    if(!flip&&m.det()<0.0)return false;

    m.mat[0][0]=workPosition[1].x;m.mat[1][0]=workPosition[1].y;m.mat[2][0]=1.0;
    m.mat[0][1]=workPosition[2].x;m.mat[1][1]=workPosition[2].y;m.mat[2][1]=1.0;
    if(flip&&m.det()>0.0)return false;
    if(!flip&&m.det()<0.0)return false;

    m.mat[0][0]=workPosition[2].x;m.mat[1][0]=workPosition[2].y;m.mat[2][0]=1.0;
    m.mat[0][1]=workPosition[0].x;m.mat[1][1]=workPosition[0].y;m.mat[2][1]=1.0;
    if(flip&&m.det()>0.0)return false;
    if(!flip&&m.det()<0.0)return false;

    return true;
}

void Render::setStencil(vec4 v){
    int x=v.x;
    int y=v.y;
    if(x<0||y<0||x>=sizeX||y>=sizeY)return;
    stencil[x][y]=true;
}
void Render::stencilReset(){
    for(int i=0;i<sizeX;i++)
    for(int j=0;j<sizeY;j++)
        stencil[i][j]=false;
}

float Render::getFloat(){
    return static_cast<float>(rand())/static_cast<float>(RAND_MAX);
}
// vec4.x is the minimum X
// vec4.x is the maximum X
// vec4.y is the minimum Y
// vec4.y is the maximum Y
vec4 Render::getBound(vec4* position){
    vec4 bound(10000,-10000,10000,-10000);
    for(int i=0;i<3;i++){
        if(bound.x>position[i].x)bound.x=position[i].x;
        if(bound.y<position[i].x)bound.y=position[i].x;
        if(bound.z>position[i].y)bound.z=position[i].y;
        if(bound.w<position[i].y)bound.w=position[i].y;
    }
    return bound;
}

void Render::addPixel(vec4 position){
    if(contains(position))return;
    qPixels.push(position);
}

void Render::addNeighbors(vec4 position){
    if(position.x<0||position.y<0||position.x>=sizeX||position.y>=sizeY)return;
    int dx[]={1,0,-1, 0};
    int dy[]={0,1, 0,-1};
    for(int i=0;i<4;i++){
        vec4 newPixel(position.x+dx[i],
                      position.y+dy[i],
                      position.z,
                      0.0);
        if(!contains(newPixel)){
            setStencil(newPixel);
            qPixels.push(newPixel);
        }
    }
}
vec4 Render::getNormal(vec4 v){
    vec4 coord=getBary(v);
    vec4 c=workNormal[0]*coord.x+
           workNormal[1]*coord.y+
           workNormal[2]*coord.z;
    return c;
}
vec4 Render::getFrag(vec4 v){
    vec4 coord=getBary(v);
    vec4 c=workColor[0]*coord.x+
           workColor[1]*coord.y+
           workColor[2]*coord.z;
    return c;
}
float Render::getDepth(vec4 v){
    vec4 coord=getBary(v);
    vec4 c=workPosition[0]*coord.x+
           workPosition[1]*coord.y+
           workPosition[2]*coord.z;
    return c.z;
}

vec4 Render::getBary(vec4 v){
    vec4* p=workPosition;
    //barycentric coordiantes - Wikipedia
    float d1,d2,d3;
    d1=((p[1].y-p[2].y)*(v.x   -p[2].x)+(p[2].x-p[1].x)*(v.y   -p[2].y))/
       ((p[1].y-p[2].y)*(p[0].x-p[2].x)+(p[2].x-p[1].x)*(p[0].y-p[2].y));
    d2=((p[2].y-p[0].y)*(v.x   -p[2].x)+(p[0].x-p[2].x)*(v.y   -p[2].y))/
       ((p[1].y-p[2].y)*(p[0].x-p[2].x)+(p[2].x-p[1].x)*(p[0].y-p[2].y));
    d3=1.0-d1-d2;
    return vec4(d1,d2,d3,0.0);
}
