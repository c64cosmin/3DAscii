#ifndef _VEC_4_
#define _VEC_4_
#include <cmath>
struct vec4{
    vec4(){
        x=y=z=w=0.0f;
    }
    vec4(float _x,float _y,float _z,float _w){
        x=_x;
        y=_y;
        z=_z;
        w=_w;
    }
    vec4 operator+(const vec4& v){
        return vec4(x+v.x,
                    y+v.y,
                    z+v.z,
                    w+v.w);
    }
    vec4 operator-(const vec4& v){
        return vec4(x-v.x,
                    y-v.y,
                    z-v.z,
                    w-v.w);
    }
    vec4 operator-(){
        return vec4(-x,
                    -y,
                    -z,
                    -w);
    }
    vec4 operator*(const float v){
        return vec4(x*v,
                    y*v,
                    z*v,
                    w*v);
    }
    vec4 operator*(const vec4 v){
        return vec4(x*v.x,
                    y*v.y,
                    z*v.z,
                    w*v.w);
    }
    vec4 clamp(){
        return vec4(x<0.0f?0.0f:(x>1.0f?1.0f:x),
                    y<0.0f?0.0f:(y>1.0f?1.0f:y),
                    z<0.0f?0.0f:(z>1.0f?1.0f:z),
                    w<0.0f?0.0f:(w>1.0f?1.0f:w));
    }
    vec4 pixelize(){
        return vec4((int)x,
                    (int)y,
                         z,
                         w);
    }
    vec4 normalize(){
        float d=sqrt(x*x+y*y+z*z);
        return vec4(x/d,
                    y/d,
                    z/d,
                    0.0);
    }
    float operator^(const vec4& v){
        return v.x*x+v.y*y+v.z*z;
    }
    float x;
    float y;
    float z;
    float w;
};
#endif
