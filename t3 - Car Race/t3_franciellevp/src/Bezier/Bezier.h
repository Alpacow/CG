//*****************************************************************
//
// classe para desenhar e controlar a pista responsavel pela pista
//
//*****************************************************************
#ifndef ___BEZIER__H___
#define ___BEZIER__H___

#include "ControlPoints.h"
#include "../Utils/Vector2.h"
#include "../Widgets/Alert.h"
#include <vector>

#define INDEX 101
#define INDEX_POLY 33

using namespace std;

class Bezier
{
private:
    vector<float> rgb;
    ControlPoints* cp;
    vector<Vector2> estimatedPoints;
    vector<Vector2> bezierPointsIn;
    vector<Vector2> bezierPointsOut;
    Vector2 center;
    int previousN;
    Alert** alert;

    void drawBezierCurveForPolygon();
    double bernstein(float n, float i);
    double factorial(int n);
    void updateSpeedWaySize();

public:
    int speedWayWidth;
    bool canApplyTransformations;
    bool translationMode;
    bool showHelp;
    bool raceOn;
    int scale;
    Bezier(Alert** alert);
    ~Bezier();
    ControlPoints* getControlPoints();
    vector<Vector2> getEstimatedPoints();
    Vector2 getCenterPoint();
    void Keyboard(int key);
    void checkMouseStates(int button, int x, int y, int state);
    void drawControlGraph();
    void render();
    void clearControlPoints();
    void calculatePointsBezier();
    void drawBezierCurve();
    void rotateCurve(float rad);
    void translate(int x, int y);
    void rescaleCurve(int scale);
    Vector2 checkIntersect ();
};

#endif
