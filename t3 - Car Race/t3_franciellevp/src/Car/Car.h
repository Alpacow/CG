//*******************************************************************************
//
// classe para fazer o controle e comunicacao necessaria entre as demais classes
//
//*******************************************************************************
#ifndef ___CAR__H___
#define ___CAR__H___

#include "../Bitmap/Bmp.h"

using namespace std;

class Car
{
private:
    Vector2 p1, p2, p3, p4;

    vector<float> rgb;           // controla cores passadas em RGB
    Bmp* img;                // imagem a ser manipulada
    float sumRotation;
    float degrees;
    float speed;

public:
    enum maxDegrees// rotacoes maximas
    {
        LeftArrow = 180,
        RightArrow = 360,
        UpArrow = 90,
        DownArrow = 270,
    };

    Car();
    Car(Alert** alerts);
    ~Car();
    void render(float fps);
    void rotateCar();
    void checkRotation(float maxDegrees);
    void moveCar(float fps);
    Vector2 rotatePoint(Vector2 p, Vector2 mid, float rad);
};

#endif
