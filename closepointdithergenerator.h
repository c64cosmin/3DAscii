#ifndef _CLOSE_POINT_DITHER_GEN_
#define _CLOSE_POINT_DITHER_GEN_
#include"colorgenerator.h"
#include<vector>
class ClosePointDitherGenerator : public ColorGenerator{
public:
    ClosePointDitherGenerator();
    ~ClosePointDitherGenerator();
    virtual void output(int x,int y,ColorRaw**bitmap);
private:
    void generateColors();
    const char* diether[4];
    float ditherMap[4][4];
    void putColor(int x,int y,double r,double g,double b);
    ColorData getGuess(double r,double g,double b);
    ColorData interpolate(double x,int color_a,int color_b);
    std::vector<double> rCoord;
    std::vector<double> gCoord;
    std::vector<double> bCoord;
    std::vector<int>    cCoord;
    void initColorPoints();
    void addColorPoint(double r,double g,double b,int color);
};
#endif //_CLOSE_POINT_DITHER_GEN_

