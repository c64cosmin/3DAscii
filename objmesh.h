#ifndef _OBJ_MESH_
#define _OBJ_MESH_

#include "vec4.h"
#include <vector>

struct xyzPoint{
    float x,y,z;
};
struct uvPoint{
    float u,v;
};
struct face{
    int a0,b0,c0;
    int a1,b1,c1;
    int a2,b2,c2;
};

class OBJMesh{
public:
    OBJMesh();
    OBJMesh(const char* filename);
    ~OBJMesh();
    std::vector<vec4> getData(){return vertexdata;}
private:
    std::vector<int> attr;
    std::vector<vec4> vertexdata;

    std::vector<xyzPoint> mesh;
    std::vector<uvPoint> uv;
    std::vector<xyzPoint> normal;
    std::vector<face> index;
};

#endif // _OBJ_MESH_
