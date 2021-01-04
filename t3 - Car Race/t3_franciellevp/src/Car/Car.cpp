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

#define CAR_WIDTH 18
#define CAR_HEIGHT 28
#define DEGREES 5
#define SPEED 5

using namespace std;

/* Inicia os atributos necessarios
*/
Car::Car(Alert** alerts, Bezier** b)
{
    this->bezier = b;
    this->alert = alerts;
    p.push_back(Vector2 {1102, 88});
    p.push_back(Vector2 {p[0].x - CAR_WIDTH / 2, p[0].y + CAR_HEIGHT});
    p.push_back(Vector2{p[0].x + CAR_WIDTH / 2, p[0].y + CAR_HEIGHT});
    img = new Bmp(1100, 80, Utils::getImagePath("car.bmp"), alerts);
    sumRotation = 90; // inicia virado para cima
    speed = 0;
    degrees = 10;
}

Car::~Car() {}

/* Renderiza/desenha tudo que sera necessario na tela
*/
void Car::render(float fps)
{
    moveCar(fps);
    rgb = Utils::RGBtoFloat(28, 28, 28);
    CV::color(rgb[0], rgb[1], rgb[2]);
    CV::polygonFill(p);
    //img->renderBmp();
}

void Car::initRace ()
{
    if ((*bezier)->canApplyTransformations) {
        p[0] = Vector2{(*bezier)->getControlPoints()->points[0]->point - (CAR_WIDTH / 2)};
        p[1] = Vector2 {p[0].x - CAR_WIDTH / 2, p[0].y + CAR_HEIGHT};
        p[2] = Vector2{p[0].x + CAR_WIDTH / 2, p[0].y + CAR_HEIGHT};
        degrees = -90;
        sumRotation = 180;
        rotateCar();
    } else
        (*alert)->alerts.push_back(new Alert(950, 30, 200, 100, "Pista nao finalizada", Utils::WARNING, TRUE));
}

void Car::increaseSpeed()
{
    speed += SPEED;
    degrees -= 2;
}

void Car::decreaseSpeed()
{
    speed -= SPEED;
    degrees += 2;
}

void Car::moveCar(float fps)
{
    if (sumRotation == 0 || sumRotation == 360) {
        for(vector<Vector2>::size_type i = 0; i != p.size(); i++)
            p[i].x += 1/fps * speed;
    } else if (sumRotation == 90) {
        for(vector<Vector2>::size_type i = 0; i != p.size(); i++)
            p[i].y -= 1/fps * speed;
    } else if (sumRotation == 180) {
        for(vector<Vector2>::size_type i = 0; i != p.size(); i++)
            p[i].x -= 1/fps * speed;
    } else if (sumRotation == 270) {
        for(vector<Vector2>::size_type i = 0; i != p.size(); i++)
            p[i].y += 1/fps * speed;
    } else if (sumRotation > 0 && sumRotation < 90) {
        for(vector<Vector2>::size_type i = 0; i != p.size(); i++) {
            p[i].x += 1/fps * speed;
            p[i].y -= 1/fps * speed;
        }
    } else if (sumRotation > 90 && sumRotation < 180) {
        for(vector<Vector2>::size_type i = 0; i != p.size(); i++) {
            p[i].x -= 1/fps * speed;
            p[i].y -= 1/fps * speed;
        }
    } else if (sumRotation > 180 && sumRotation < 270) {
        for(vector<Vector2>::size_type i = 0; i != p.size(); i++) {
            p[i].x -= 1/fps * speed;
            p[i].y += 1/fps * speed;
        }
    } else if (sumRotation > 270 && sumRotation < 360) {
        for(vector<Vector2>::size_type i = 0; i != p.size(); i++) {
            p[i].x += 1/fps * speed;
            p[i].y += 1/fps * speed;
        }
    }
}

void Car::rotateCar() {
    float rad = degrees * PI / 180; // transforma graus para radianos
    Vector2 mid = (p[0] + p[1] + p[2]) / 3;
    for(vector<Vector2>::size_type i = 0; i != p.size(); i++)
        p[i] = Utils::rotatePoint(p[i], mid, rad);
    //img->rotateImage(rad);
}

void Car::checkRotation(float maxDegrees)
{
    float validDegrees = 0;
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
    degrees = validDegrees;
    rotateCar();
}
