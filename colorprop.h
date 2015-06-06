#ifndef _COLOR_PROP_
#define _COLOR_PROP_
#define RED 1
#define GREEN 2
#define BLUE 4
#define BRIGHT 8
#define PLAIN_COLOR 7

struct ColorData{
    char diether;
    char bg;
    char fg;
};

struct ColorRaw{
    int r;
    int g;
    int b;
};
#endif //_COLOR_PROP_
