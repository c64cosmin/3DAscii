#ifndef _MAT_3_
#define _MAT_3_
struct mat3{
    float det(){
        float sum=0;
        for(int j=0;j<3;j++){
            float prodp=1;
            float prodn=1;
            for(int i=0;i<3;i++){
                prodp*=mat[(j+i)%3][i];
                prodn*=mat[(j-i+3)%3][i];
            }
            sum+=prodp-prodn;
        }
        return sum;
    }
    float mat[3][3];
};
#endif //_MAT_3_
