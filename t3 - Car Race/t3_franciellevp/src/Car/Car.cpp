//*******************************************************************************
//
// classe para fazer o controle e comunicacao necessaria entre as demais classes
//
//*******************************************************************************
#include "Car.h"
#include "../Utils/Utils.h"
#include "../Canvas/gl_canvas2d.h"
#include <iostream>
#include <math.h>

#define CAR_WIDTH 22
#define CAR_HEIGHT 35
#define DEGREES 10

using namespace std;

/* Inicia os atributos necessarios
*/
Car::Car(Alert** alerts)
{
    p1 = Vector2 {1105, 86};
    p2 = {p1.x + CAR_WIDTH, p1.y};
    p3 = Vector2{p1.x, p1.y + CAR_HEIGHT};
    p4 = Vector2{p1.x + CAR_WIDTH, p1.y + CAR_HEIGHT};
    img = new Bmp(Utils::getImagePath("car.bmp"), alerts);
    sumRotation = 90; // inicia virado para cima
    speed = 10;
    degrees = 10;
}

Car::~Car() {}

/* Renderiza/desenha tudo que sera necessario na tela
*/
void Car::render()
{
    img->renderBmp(1100, 80);
    CV::rectFill(p1, p2, p3, p4);
}

Vector2 Car::rotatePoint(Vector2 p, Vector2 mid, float rad) {
    float a = p.x - mid.x;
    float b = p.y - mid.y;
    float xx = +a * cos(rad) - b * sin(rad) + mid.x;
    float yy = +a * sin(rad) + b * cos(rad) + mid.y;
    return Vector2{xx, yy};
}

void Car::rotateRect() {
    float rad = degrees * PI / 180; // transforma graus para radianos
    Vector2 mid = p1 + ((p4 - p1) / 2);
    p1 = rotatePoint(p1, mid, rad);
    p2 = rotatePoint(p2, mid, rad);
    p3 = rotatePoint(p3, mid, rad);
    p4 = rotatePoint(p4, mid, rad);
}

void Car::checkRotation(float maxDegrees)
{
    float validDegrees = 0;
    cout << "Angulo atual = " << sumRotation << endl;
    if (maxDegrees == RightArrow) {
        if (sumRotation > 0 && sumRotation <= 90) { // 1 quadrante
            sumRotation -= DEGREES; // pra direita horario
            if (sumRotation < 0) validDegrees += DEGREES; // anda so o necessario
            else validDegrees = DEGREES;
        } else if (sumRotation >= 270 && sumRotation < 360) { // 4 quadrante
            sumRotation += DEGREES; // pra direita antihorario
            if (sumRotation > 360) validDegrees = sumRotation - 360; // anda so o necessario
            else validDegrees = -DEGREES;
        }
    } else if (maxDegrees == LeftArrow) {
        if (sumRotation >= 90 && sumRotation < 180) { // 2 quadrante
            sumRotation += DEGREES; // pra esquerda antihorario
            if (validDegrees > 180) validDegrees -= 180; // anda so o necessario
            else validDegrees = -DEGREES;
        } else if (sumRotation > 180 && sumRotation <= 270) { // 3 quadrante
            sumRotation -= DEGREES; // pra esquerda horario
            if (sumRotation < 180) validDegrees = 180 - sumRotation; // anda so o necessario
            else validDegrees = DEGREES;
        }
    } else if (maxDegrees == UpArrow) {
        if (sumRotation == 360) sumRotation = 0;
        if (sumRotation > 90 && sumRotation <= 180) { // 2 quadrante
            sumRotation -= DEGREES; // pra direita horario
            if (sumRotation < 90) validDegrees = 90 - validDegrees; // anda so o necessario
            else validDegrees = DEGREES;
        } else if (sumRotation < 90 && sumRotation >= 0) { // 1 quadrante
            sumRotation += DEGREES; // pra direita antihorario
            if (sumRotation > 90) validDegrees = sumRotation - 90; // anda so o necessario
            else validDegrees = -DEGREES;
        }
    } else if (maxDegrees == DownArrow) {
        if (sumRotation == 0) sumRotation = 360;
        if (sumRotation >= 180 && sumRotation < 270) { // 3 quadrante
            sumRotation += DEGREES; // pra direita antihorario
            if (sumRotation > 270) validDegrees = sumRotation - 270; // anda so o necessario
            else validDegrees = -DEGREES;
        } else if (sumRotation > 270 && sumRotation <= 360) { // 4 quadrante
            sumRotation -= DEGREES; // pra esquerda horario
            if (sumRotation < 270) validDegrees = 270 - sumRotation; // anda so o necessario
            else validDegrees = DEGREES;
        }
    }
    cout << sumRotation << " andou " << validDegrees << endl;
    degrees = validDegrees;
}
