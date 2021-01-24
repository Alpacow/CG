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
    vector<Vector3> points;
    vector<Vector3> points2D;
    vector<Vector3> lines;
    vector<Vector3> lines2D;
    vector<float> color;
    Vector3 origin;
    Vector3 translation;

    void drawGear2D();
    void initDraw2D(float iniPoint, float increment, float rad, int frontBack);
    void draw2D();
    void rotate3D(int axis);
    Vector3 calcToothPosition(float ang, float radius, float z);

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
