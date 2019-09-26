#ifndef POINT_H
#define POINT_H
#include <vector>


class Point
{
public:
    Point() {
        val = std::vector<float>(3,0.0);
    }

    Point(float a,float b, float c){
        val = std::vector<float>(3,0.0);
        val[0]=a;
        val[1]=b;
        val[2]=c;
    }

    Point(float a,float b){
        val = std::vector<float>(3,0.0);
        val[0]=a;
        val[1]=b;
        val[2]=0.0;
    }

    std::vector<float> val;

};

#endif // POINT_H
