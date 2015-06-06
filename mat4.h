#ifndef _MAT_4_
#define _MAT_4_

#include "vec4.h"

struct mat4{
    mat4(){
        for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            mat[i][j]=0;
    }
    mat4(vec4 a,vec4 b,vec4 c,vec4 d){
        mat[0][0]=a.x;mat[1][0]=a.y;mat[2][0]=a.z;mat[3][0]=a.w;
        mat[0][1]=b.x;mat[1][1]=b.y;mat[2][1]=b.z;mat[3][1]=b.w;
        mat[0][2]=c.x;mat[1][2]=c.y;mat[2][2]=c.z;mat[3][2]=c.w;
        mat[0][3]=d.x;mat[1][3]=d.y;mat[2][3]=d.z;mat[3][3]=d.w;
    }
    mat4 operator*(const mat4& m){
        mat4 r;
        for(int y=0;y<4;y++)
        for(int x=0;x<4;x++){
            float sum=0;
            for(int i=0;i<4;i++)
                sum+=mat[i][y]*m.mat[x][i];
            r.mat[x][y]=sum;
        }
        return r;
    }
    vec4 operator*(const vec4& v){
        vec4 r;
        float u[4];
        u[0]=v.x;
        u[1]=v.y;
        u[2]=v.z;
        u[3]=v.w;
        for(int x=0;x<4;x++){
            r.x+=mat[x][0]*u[x];
            r.y+=mat[x][1]*u[x];
            r.z+=mat[x][2]*u[x];
            r.w+=mat[x][3]*u[x];
        }
        return r;
    }
    float mat[4][4];
    static mat4 translate(float x,float y,float z){
        return mat4(vec4(1.0,0.0,0.0,x  ),
                    vec4(0.0,1.0,0.0,y  ),
                    vec4(0.0,0.0,1.0,z  ),
                    vec4(0.0,0.0,0.0,1.0));
    }
    static mat4 scale(float x,float y,float z){
        return mat4(vec4(x  ,0.0,0.0,0.0),
                    vec4(0.0,y  ,0.0,0.0),
                    vec4(0.0,0.0,z  ,0.0),
                    vec4(0.0,0.0,0.0,1.0));
    }
    static mat4 rotation(float x,float y,float z,float a){
        a=a*3.1415926536/180.0;
        float c=cos(a);
        float s=sin(a);
        return mat4(vec4(x*x*(1-c)+c  ,x*y*(1-c)-z*s,x*z*(1-c)+y*s,0),
                    vec4(x*y*(1-c)+z*s,y*y*(1-c)+c  ,y*z*(1-c)-x*s,0),
                    vec4(x*z*(1-c)-y*s,y*z*(1-c)+x*s,z*z*(1-c)+c,  0),
                    vec4(0,0,0,1));
    }
    static mat4 ortho(float l,float r,float t,float b,float f,float n){
        return mat4(vec4(2.0/(r-l),0,0,-(r+l)/(r-l)),
                    vec4(0,2.0/(t-b),0,-(t+b)/(t-b)),
                    vec4(0,0,-2.0/(f-n),(f+n)/(f-n)),
                    vec4(0,0,0,1));
    }
    static mat4 persp(float fov,float aspect,float n,float f){
        float ff=1.0/tan(fov/2.0);
        /*return mat4(vec4(ff/aspect,0,0,0),
                    vec4(0,ff,0,0),
                    vec4(0,0,(n+f)/(n-f),-1),
                    vec4(0,0,(2*n*f)/(n-f),0));//trasnpose*/
        return mat4(vec4(ff/aspect,0,0,0),
                    vec4(0,ff,0,0),
                    vec4(0,0,(n+f)/(n-f),(2*n*f)/(n-f)),
                    vec4(0,0,-1,0));

    }
    static mat4 screen(float x,float y){
        return mat4::translate(-x/2,-y/2,0.0)*mat4::scale(x,y,1.0);
    }
};
#endif //_MAT_4_ſ
