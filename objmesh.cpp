#include "objmesh.h"
#include <cstdio>

OBJMesh::OBJMesh(){
}
OBJMesh::OBJMesh(const char* filename){
    char* buffer=new char[256];
    FILE* f=fopen(filename,"r");

    while(!feof(f)){
        char* test=fgets(buffer,256,f);
        if(!test)break;

        if(buffer[0]=='v'){
            if(buffer[1]==' '){
                xyzPoint dummy;
                sscanf(buffer+2,"%f %f %f",&dummy.x,&dummy.y,&dummy.z);

                mesh.push_back(dummy);
            }else if(buffer[1]=='t'){
                uvPoint dummy;
                sscanf(buffer+3,"%f %f",&dummy.u,&dummy.v);

                uv.push_back(dummy);
            }else if(buffer[1]=='n'){
                xyzPoint dummy;
                sscanf(buffer+3,"%f %f %f",&dummy.x,&dummy.y,&dummy.z);

                normal.push_back(dummy);
            }
        }
        if(buffer[0]=='f'){
            face dummy;
            sscanf(buffer+2,"%i/%i/%i %i/%i/%i %i/%i/%i",&dummy.a0,&dummy.a1,&dummy.a2,
                                                         &dummy.b0,&dummy.b1,&dummy.b2,
                                                         &dummy.c0,&dummy.c1,&dummy.c2);

            index.push_back(dummy);
        }
    }
    fclose(f);

    for(int i=0;i<index.size();i++){
        vec4 v;
        v=vec4(mesh[index[i].a0 -1].x,
               mesh[index[i].a0 -1].y,
               mesh[index[i].a0 -1].z,
               1.0);
        vertexdata.push_back(v);
        v=vec4(mesh[index[i].b0 -1].x,
               mesh[index[i].b0 -1].y,
               mesh[index[i].b0 -1].z,
               1.0);
        vertexdata.push_back(v);
        v=vec4(mesh[index[i].c0 -1].x,
               mesh[index[i].c0 -1].y,
               mesh[index[i].c0 -1].z,
               1.0);
        vertexdata.push_back(v);

        v=vec4(normal[index[i].a2 -1].x,
               normal[index[i].a2 -1].y,
               normal[index[i].a2 -1].z,
               0.0);
        vertexdata.push_back(v);
        v=vec4(normal[index[i].b2 -1].x,
               normal[index[i].b2 -1].y,
               normal[index[i].b2 -1].z,
               0.0);
        vertexdata.push_back(v);
        v=vec4(normal[index[i].c2 -1].x,
               normal[index[i].c2 -1].y,
               normal[index[i].c2 -1].z,
               0.0);
        vertexdata.push_back(v);
        //vertexdata.push_back(  uv[index[i].aa-1].u);
        //vertexdata.push_back(  uv[index[i].aa-1].v);
        //vertexdata.push_back(  uv[index[i].bb-1].u);
        //vertexdata.push_back(  uv[index[i].bb-1].v);
        //vertexdata.push_back(  uv[index[i].cc-1].u);
        //vertexdata.push_back(  uv[index[i].cc-1].v);
    }
}

OBJMesh::~OBJMesh(){

}
