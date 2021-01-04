#ifndef __CONTROLPOINTS_H__
#define __CONTROLPOINTS_H__

#include "../Canvas/gl_canvas2d.h"
#include <math.h>
#include <vector>

#define R 4
#define DRAG_DIST 20
#define DRAW_Y_VALID 30
#define DRAW_X_VALID 1020

using namespace std;

class ControlPoints
{
private:
    Vector2 dragPoint;
    Vector2 fstPoint;
    int nPoint;
    char* label;
    void drawDragPoints();

public:
    Vector2 point;
    vector<ControlPoints*> points;  // vetor com todos os pontos de controle
    bool canDragPoint;
    bool isSelect;

    ControlPoints(int x, int y, int pn);
    ControlPoints();
    ~ControlPoints();
    ControlPoints* checkCollisionDragPoints(int x, int y);
    Vector2 getDragPoint();
    Vector2 getFstPoint();
    void render();
    void dragSelectPoint(int mx, int my);
    void addPoint(int x, int y);
    void drawControlGraph();
    void clearControlPoints();
    bool checkControlPointArea(int x, int y);
    bool checkCollisionFirstPoint();
    void unsetAllPoints();
};

#endif

