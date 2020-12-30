
//*******************************************************************************
//
// classe para fazer o controle e comunicacao necessaria entre as demais classes
//
//*******************************************************************************
#ifndef ___BEZIER__H___
#define ___BEZIER__H___

#include "ControlPoints.h"
#include "../Utils/Vector2.h"
#include <vector>

using namespace std;

class Bezier
{
private:
    vector<float> rgb;           // controla cores passadas em RGB
    float t; // t da equação de bezier (TODO: verificar se sera preciso)
    ControlPoints* cp;
    bool canDrawSpeedWay;
    int speedWayWidth;
    vector<Vector2> cpIn;
    vector<Vector2> cpOut;
    vector<Vector2> bezierPointsIn;
    vector<Vector2> bezierPointsOut;

    void drawBezierCurveForPolygon();
    double bernstein(float n, float i);
    double factorial(int n);
public:

    Bezier();
    ~Bezier();
    ControlPoints* getControlPoints();
    void setCanDragSpeedWay(bool value);
    void Keyboard(int key);
    void checkMouseStates(int button, int x, int y, int state);
    void drawControlGraph();
    void render();
    void clearControlPoints();
    void drawBezierCurve(float maxValue);
};

#endif
