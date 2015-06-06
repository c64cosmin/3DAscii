#ifndef _PPM_GENERATOR_
#define _PPM_GENERATOR_
#include "colorgenerator.h"
class PPMGenerator : public ColorGenerator{
public:
    PPMGenerator();
    ~PPMGenerator();
    virtual void output(int x,int y,ColorRaw**bitmap);
};
#endif //_PPM_GENERATOR_
