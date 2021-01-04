//*******************************************************************************
//
// classe para fazer o controle e comunicacao necessaria entre as demais classes
//
//*******************************************************************************
#ifndef ___CAR__H___
#define ___CAR__H___

#include "../Bitmap/Bmp.h"
#include "../Bezier/Bezier.h"
#include "../Widgets/Alert.h"

using namespace std;

class Car
{
private:
    vector<Vector2> p;

    vector<float> rgb;          // controla cores passadas em RGB
    Bmp* img;                // imagem a ser manipulada
    Bezier** bezier;
    Alert** alert;
    float sumRotation;
    float degrees;
    float speed;

public:
    vector<float> carColor;   // controla cores passadas em RGB
    enum maxDegrees// rotacoes maximas
    {
        LeftArrow = 180,
        RightArrow = 360,
        UpArrow = 90,
        DownArrow = 270,
    };

    Car();
    Car(Alert** alerts, Bezier** b);
    ~Car();
    void render(float fps);
    void rotateCar();
    void checkRotation(float maxDegrees);
    void moveCar(float fps);
    void increaseSpeed();
    void decreaseSpeed();
    void initRace();
};

#endif
