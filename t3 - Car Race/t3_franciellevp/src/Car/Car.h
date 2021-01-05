//********************************************************************
//
// classe para fazer o controle e movimentacao do carrinho e do jogo
//
//********************************************************************
#ifndef ___CAR__H___
#define ___CAR__H___

#include "../Bitmap/Bmp.h"
#include "../Bezier/Bezier.h"
#include "../Widgets/Alert.h"

#define CAR_WIDTH 18
#define CAR_HEIGHT 28
#define DEGREES 5
#define SPEED 5
#define COIN_RAD 6
#define COIN_FINISH 1

using namespace std;

class Car
{
private:
    vector<Vector2> p;

    vector<float> rgb;          // controla cores passadas em RGB
    Bezier** bezier;
    Alert** alert;
    float sumRotation;
    float degrees;
    float speed;
    bool isCoinOn;
    Vector2 coinPos;
    int coinCount;

public:
    vector<float> carColor;   // controla cores passadas em RGB
    bool finish;
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
    vector<Vector2> rotateCar(vector<Vector2> pCar);
    void checkRotation(float maxDegrees);
    void moveCar(float fps);
    void increaseSpeed();
    void decreaseSpeed();
    void initRace();
    void checkCoinCollision();
};

#endif
