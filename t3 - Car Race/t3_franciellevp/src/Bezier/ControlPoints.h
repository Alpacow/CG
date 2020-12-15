#ifndef __CONTROLPOINTS_H__
#define __CONTROLPOINTS_H__

#include "../Canvas/gl_canvas2d.h"
#include <math.h>
#include <vector>

#define R 4
#define DRAG_DIST 20

using namespace std;

class ControlPoints
{
private:
    Vector2 p;
    int pn;
    char* label;
    bool isSelect;
    void drawDragPoints();
    vector<float> rgb;

public:
    ControlPoints(int x, int y, int pn);
    ~ControlPoints();
    void render();
    Vector2 getPoint();
    void setIsSelect(bool value);
    void dragPoint(int mx, int my);
    bool checkCollision(int mx, int my, Vector2 p);
};

#endif

