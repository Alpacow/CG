//************************************************************
//
// classe para renderizar e realizar operacoes da engrenagem
//
//************************************************************
#ifndef ___GEAR__H___
#define ___GEAR__H___

#include "../Widgets/Alert.h"
#include "../Utils/Vector3.h"
#include <vector>

using namespace std;

class Gear
{
private:
    vector<float> rgb;           // controla cores passadas em RGB
    float x;
    float y;
    float z;
    vector<Vector3> points;
    vector<Vector3> points2D;
    vector<Vector3> lines;
    vector<Vector3> lines2D;
    vector<float> color;
    Vector3 origin;

    void drawGear2D();
    void initDraw2D(float iniPoint, float increment, float translationX, float translationY, float rad, int frontBack);
    void draw2D();
    Vector3 calcToothPosition(float ang, float radius, float z, float tx, float ty);

public:
    Alert* alerts;
    float radius;
    float radiusOut;
    float angTeeth;
    float width;
    int nTeeth;
    int nFaces;
    float velRotation;
    float velTranslation;

    Gear(float rad, float angTeeth, int nTeeth, int nFaces, vector<float> color);
    Gear();
    ~Gear();
    void render();
};

#endif
