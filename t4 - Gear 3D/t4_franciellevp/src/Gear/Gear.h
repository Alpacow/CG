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
    float angTeeth;
    float radiusOut;

    void drawGear2D();
    void initDraw2D(int* i, int* j, bool frontBack);
    void rotate3D(int axis, float rad);
    void initGear();
    Vector3 calcToothPosition(float ang, float radius, float z);
    void updateArraysSize();

public:
    Alert* alerts;
    float radius;
    float width;
    int nTeeth;
    int nFaces;
    float velRotation;
    float velTranslation;
    int screenDist;

    Gear(float rad, int nTeeth, int nFaces, vector<float> color, float x, float y, float z);
    Gear();
    ~Gear();
    void render();
    void MoveX(float dist);
    void MoveY(float dist);
    void MoveZ(float dist);
    void setWidth(float value);
    void setNroTeeth(int value);
};

#endif
