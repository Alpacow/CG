
//*******************************************************************************
//
// classe para fazer o controle e comunicacao necessaria entre as demais classes
//
//*******************************************************************************
#ifndef ___BEZIER__H___
#define ___BEZIER__H___

#include "ControlPoints.h"
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
    double bernstein(float n, float i);
    double factorial(int n);
};

#endif
