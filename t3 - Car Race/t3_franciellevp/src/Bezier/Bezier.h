
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
    vector<ControlPoints*> cp;  // vetor com todos os pontos de controle
    ControlPoints* fstPoint;

    bool checkCollisionAll(int x, int y);
    void unsetAllPoints();
public:
    Bezier();
    ~Bezier();
    void Keyboard(int key);
    void checkMouseStates(int button, int x, int y, int state);
    void drawControlGraph();
    void render();
};

#endif
