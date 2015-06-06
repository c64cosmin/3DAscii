#include "xpmgenerator.h"
#include <cstdio>

XPMGenerator::XPMGenerator()
             :ColorGenerator(){
}

XPMGenerator::~XPMGenerator(){
}

void XPMGenerator::output(int x,int y,ColorRaw**bitmap){
    if(!bitmap)return;
    colorPalette.resize(0);
    for(int j=0;j<y;j++)
    for(int i=0;i<x;i++)
        add(getCode(bitmap[i][j]));

    printf("/* XPM */\n");
    printf("static char *egc[] = {\n");
    printf("/* width,height,nrcolors,charsperpixel */\n");
    printf("\" %i %i %i 7 \"\n",x,y,colorPalette.size());
    printf("/* colors #RRGGBB */\n");
    for(int i=0;i<colorPalette.size();i++)
        printf("\"%s c %s\",",colorPalette[i].c_str(),colorPalette[i].c_str());
    printf("\n");
    for(int j=0;j<y;j++){
        printf(",\n\"");
        for(int i=0;i<x;i++){
            printf("%s",getCode(bitmap[i][j]).c_str());
        }
        printf("\"");
    }
    printf("};\n");
}

std::string XPMGenerator::getCode(ColorRaw c){
    std::string hexcolor="#000000";
    hexcolor[1]=hexDigit(c.r/16);
    hexcolor[2]=hexDigit(c.r%16);

    hexcolor[3]=hexDigit(c.g/16);
    hexcolor[4]=hexDigit(c.g%16);

    hexcolor[5]=hexDigit(c.b/16);
    hexcolor[6]=hexDigit(c.b%16);
    return hexcolor;
}

char XPMGenerator::hexDigit(int x){
    if(x<0||x>15)return '!';//this is bad, this is error
    if(x>=0&&x<=9)return x+'0';
    return x-10+'A';
}

void XPMGenerator::add(std::string color){
    for(int i=0;i<colorPalette.size();i++)
        if(!color.compare(colorPalette[i]))return;
    colorPalette.push_back(color);
}
