#ifndef _CLOSE_POINT_GEN_
#define _CLOSE_POINT_GEN_
#include"colorgenerator.h"
#include<vector>
class ClosePointGenerator : public ColorGenerator{
public:
    ClosePointGenerator();
    ~ClosePointGenerator();
    virtual void output(int x,int y,ColorRaw**output);
private:
    void generateColors();
    const char* diether[4];
    ColorData colordata[16][16][16];
    ColorData getGuess(double r,double g,double b);
    ColorData convert(int color);
    std::vector<double> rCoord;
    std::vector<double> gCoord;
    std::vector<double> bCoord;
    std::vector<int>    cCoord;
    void initColorPoints();
    void addColorPoint(double r,double g,double b,int color);
    void putColor(double r,double g,double b);
};
#endif //_CLOSE_POINT_GEN_
