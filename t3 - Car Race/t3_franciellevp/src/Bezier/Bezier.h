
//*******************************************************************************
//
// classe para fazer o controle e comunicacao necessaria entre as demais classes
//
//*******************************************************************************
#ifndef ___BEZIER__H___
#define ___BEZIER__H___

#include "ControlPoints.h"
#include "../Widgets/Slider.h"
#include "../Utils/Vector2.h"
#include <vector>

using namespace std;

class Bezier
{
private:
    vector<float> rgb;           // controla cores passadas em RGB
    float t; // t da equação de bezier (TODO: verificar se sera preciso)
    ControlPoints* cp;
    vector<Vector2> estimatedPoints;
    vector<Vector2> bezierPointsIn;
    vector<Vector2> bezierPointsOut;
    Slider* slider;

    void drawBezierCurveForPolygon();
    double bernstein(float n, float i);
    double factorial(int n);

public:
    int speedWayWidth;
    bool canApplyTransformations;
    Bezier(Slider* slider);
    ~Bezier();
    ControlPoints* getControlPoints();
    void Keyboard(int key);
    void checkMouseStates(int button, int x, int y, int state);
    void drawControlGraph();
    void render();
    void clearControlPoints();
    void drawBezierCurve(float maxValue);
    void getPointsBezier();
    Vector2 getCenterPoint();
    void rotateCurve(float rad);
};

#endif
