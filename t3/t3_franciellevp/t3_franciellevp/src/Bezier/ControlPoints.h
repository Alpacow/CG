#ifndef __CONTROLPOINTS_H__
#define __CONTROLPOINTS_H__

#include "../Canvas/gl_canvas2d.h"
#include <math.h>
#include <string>

#define R 4

using namespace std;

class ControlPoints
{
private:
    Vector2 p;
    int pn;
    char* label;
    bool isSelect;

public:
    ControlPoints(int x, int y, int pn);
    ~ControlPoints();
    void render();
    Vector2 getPoint();
    void setIsSelect(bool value);
};

#endif

