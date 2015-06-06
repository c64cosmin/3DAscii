#ifndef _COLOR_GENERATOR_
#define _COLOR_GENERATOR_

class Ascii;
#include "colorprop.h"

class ColorGenerator{
public:
    ColorGenerator();
    ~ColorGenerator();
    virtual void output(int x,int y,ColorRaw**bitmap);
friend class Ascii;
};
#endif //_COLOR_GENERATOR_
