#ifndef _COLOR_H_
#define _COLOR_H_

#include <vector>
#include "colorprop.h"
#include "vec4.h"
#include "render.h"
#include "objmesh.h"

class ColorGenerator;

class Ascii{
public:
    Ascii(int sizeX,int sizeY,const char* model);
    ~Ascii();
    void bitblit(ColorGenerator* generator);
    void doSomething(int t=0);
    void doPlasma();
    double func(int x,int y,int t);
    double func2(int x,int y,int t,double n);
    void setNoDepth(bool s){noDepth=s;}
private:
    bool noDepth;
    void putLineX(vec4 positionA,vec4 positionB,vec4 colorA,vec4 colorB);
    void putLineY(vec4 positionA,vec4 positionB,vec4 colorA,vec4 colorB);
    void putLineD(vec4 positionA,vec4 positionB,vec4 colorA,vec4 colorB);
    void initMap();
    ColorRaw** bitmap;
    float** depth;
    Render* render;
    int sizeX,sizeY;
    void clearDepth();
    void clearBitmap();
    void putColor(vec4 position, vec4 color);
    void putColorZ(vec4 position, vec4 color);
    void putLine(vec4* position, vec4* color,int ra=0,int rb=1);
    void putTriple(vec4* position, vec4* color);
    void putTripleFill(vec4* position, vec4* color, vec4* normal);
    OBJMesh obj;
};
#endif //_COLOR_H_
