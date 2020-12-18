#ifndef __CONTROLPOINTS_H__
#define __CONTROLPOINTS_H__

#include "../Canvas/gl_canvas2d.h"
#include <math.h>
#include <vector>

#define R 4
#define DRAG_DIST 20
#define DRAW_Y_VALID 60
#define DRAW_X_VALID 1030

using namespace std;

class ControlPoints
{
private:
    Vector2 point;
    Vector2 fstPoint;
    int nPoint;
    char* label;
    bool isSelect;
    bool canDragPoint;
    void drawDragPoints();

public:
    vector<ControlPoints*> points;  // vetor com todos os pontos de controle

    ControlPoints(int x, int y, int pn);
    ControlPoints();
    ~ControlPoints();
    int checkCollisionAll(int x, int y);
    Vector2 getPoint();
    Vector2 getFirstPoint();
    void render();
    void setIsSelect(bool value);
    void dragPoint(int mx, int my);
    void unsetAllPoints();
    void addPoint(int x, int y);
    void drawControlGraph();
    void clearControlPoints();
    void setCanDragPoint(bool value);
};

#endif

