#ifndef _XPM_GENERATOR_
#define _XPM_GENERATOR_

#include "colorgenerator.h"
#include <vector>
#include <string>

class XPMGenerator : public ColorGenerator{
public:
    XPMGenerator();
    ~XPMGenerator();
    virtual void output(int x,int y,ColorRaw**bitmap);
private:
    std::vector<std::string> colorPalette;
    std::string getCode(ColorRaw c);
    char hexDigit(int x);
    void add(std::string color);
};

#endif //_XPM_GENERATOR_
