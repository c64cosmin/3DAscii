#ifndef _RENDER_
#define _RENDER_
#include "vec4.h"
#include <queue>
#include <vector>
class Render{
public:
    Render(int sizeX,int sizeY);
    ~Render();
    void workTriple(vec4* position, vec4* color,vec4* normal);
    void getPixel(vec4 &p,vec4 &c);
    void stencilReset();
    bool done(){return qPixels.empty();}
private:
    int sizeX;
    int sizeY;
    void setStencil(vec4 v);
    bool** stencil;
    std::queue<vec4> qPixels;
    std::queue<vec4*> qTriples;
    float getFloat();
    bool contains(vec4 v);
    vec4 getBound(vec4* position);
    void addNeighbors(vec4 postion);
    void addPixel(vec4 position);
    bool onTriangle(vec4 v);
    bool onBound(vec4 v);
    vec4* workPosition;
    vec4* workColor;
    vec4* workNormal;
    vec4 getFrag(vec4 p);
    vec4 getFragShade(vec4 p);
    float getDepth(vec4 p);
    vec4 getNormal(vec4 p);
    vec4 getBary(vec4 p);
};
#endif // _RENDER_
