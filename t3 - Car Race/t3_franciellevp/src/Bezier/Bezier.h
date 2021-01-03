
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

#define INDEX 101
#define INDEX_POLY 33

using namespace std;

class Bezier
{
private:
    vector<float> rgb;           // controla cores passadas em RGB
    ControlPoints* cp;
    vector<Vector2> estimatedPoints;
    vector<Vector2> bezierPointsIn;
    vector<Vector2> bezierPointsOut;
    Slider* slider;
    Vector2 center;
    Vector2 lastPosition;

    void drawBezierCurveForPolygon();
    double bernstein(float n, float i);
    double factorial(int n);
    void updateSpeedWaySize();

public:
    int speedWayWidth;
    bool canApplyTransformations;
    bool translationMode;
    bool scaleMode;
    Bezier(Slider* slider);
    ~Bezier();
    ControlPoints* getControlPoints();
    void Keyboard(int key);
    void checkMouseStates(int button, int x, int y, int state);
    void drawControlGraph();
    void render();
    void clearControlPoints();
    void drawBezierCurve();
    void getPointsBezier();
    Vector2 getCenterPoint();
    void rotateCurve(float rad);
    void translate(int x, int y);
    void rescaleCurve(Vector2 scale);
};

#endif
