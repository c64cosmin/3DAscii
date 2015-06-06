#ifndef _CLOSE_POINT_INTERPOLATE_GEN_
#define _CLOSE_POINT_INTERPOLATE_GEN_
#include"colorgenerator.h"
#include<vector>
#define GUESS_RESOLUTION 64
class ClosePointInterpolateGenerator : public ColorGenerator{
public:
    ClosePointInterpolateGenerator();
    ~ClosePointInterpolateGenerator();
    virtual void output(int x,int y,ColorRaw**bitmap);
private:
    void generateColors();
    const char* diether[4];
    ColorData colordata[GUESS_RESOLUTION][GUESS_RESOLUTION][GUESS_RESOLUTION];
    void putColor(double r,double g,double b);
    ColorData getGuess(double r,double g,double b);
    ColorData interpolate(double x,int color_a,int color_b);
    std::vector<double> rCoord;
    std::vector<double> gCoord;
    std::vector<double> bCoord;
    std::vector<int>    cCoord;
    void initColorPoints();
    void addColorPoint(double r,double g,double b,int color);
};
#endif //_CLOSE_POINT_INTERPOLATE_GEN_
