//********************************************************************
//
// classe para fazer o controle e movimentacao do carrinho e do jogo
//
//********************************************************************
#include "Car.h"
#include "../Utils/Utils.h"
#include "../Canvas/gl_canvas2d.h"
#include <iostream>
#include <math.h>
#include <ctime>

using namespace std;

/* Inicializa atributos necessarios
   @param alerts: instancia da classe Alert para manipular janelas de mensagem
   @param b: instancia da classe Bezier para manipular alguns atributos
*/
Car::Car(Alert** alerts, Bezier** b)
{
    this->bezier = b;
    this->alert = alerts;
    isCoinOn = false;
    finish = false;
    carColor = Utils::RGBtoFloat(28, 28, 28);
    p.push_back(Vector2 {1102, 88});
    p.push_back(Vector2 {p[0].x - CAR_WIDTH / 2, p[0].y + CAR_HEIGHT});
    p.push_back(Vector2{p[0].x + CAR_WIDTH / 2, p[0].y + CAR_HEIGHT});
    sumRotation = 90; // inicia virado para cima
    speed = 0;
    coinCount = 0;
    degrees = 10;
}

Car::~Car() {}

/* Renderiza/desenha tudo que sera necessario na tela
   @param fps: frames por segundo calculados
*/
void Car::render(float fps)
{
    if (!finish)
        moveCar(fps);
    CV::color(carColor[0], carColor[1], carColor[2]);
    CV::polygonFill(p);
    if ((*bezier)->raceOn && !isCoinOn) {
        srand((unsigned) time(0));
        int r = 0 + (rand() % (*bezier)->getEstimatedPoints().size() - 1);
        coinPos = (*bezier)->getEstimatedPoints()[r];
        isCoinOn = true;
    }
    if (isCoinOn) {
        char* text = (char*)malloc(sizeof(char) * 30);
        sprintf(text, "Pontuacao: %d", coinCount);
        rgb = Utils::RGBtoFloat(28, 28, 28);
        CV::color(rgb[0], rgb[1], rgb[2]);
        CV::text(1050, 100, text);
        rgb = Utils::RGBtoFloat(124,252,0);
        CV::color(rgb[0], rgb[1], rgb[2]);
        CV::circleFill(coinPos.x, coinPos.y, COIN_RAD, 10);
    }
    checkCoinCollision();
    if (coinCount == COIN_FINISH) {
        finish = true;
        speed = 0;
        coinCount = 0;
        (*alert)->alerts.push_back(new Alert(950, 30, 200, 100, "Jogo finalizado", Utils::SUCCESS, TRUE));
    }
}

/* Realiza configuracoes necessarias para iniciar a "corrida"
*/
void Car::initRace ()
{
    if ((*bezier)->canApplyTransformations) {
        isCoinOn = false;
        finish = false;
        speed = 0;
        coinCount = 0;
        p[0] = Vector2{(*bezier)->getControlPoints()->points[0]->point - (CAR_WIDTH / 2)};
        p[1] = Vector2 {p[0].x - CAR_WIDTH / 2, p[0].y + CAR_HEIGHT};
        p[2] = Vector2{p[0].x + CAR_WIDTH / 2, p[0].y + CAR_HEIGHT};
        degrees = -90;
        sumRotation = 180;
        p = rotateCar(p);
    } else
        (*alert)->alerts.push_back(new Alert(950, 30, 200, 100, "Pista nao finalizada", Utils::WARNING, TRUE));
}

/* Renderiza/desenha tudo que sera necessario na tela
   @param fps: frames por segundo calculados
*/
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
    } else if (sumRotation > 90 && sumRotation < 180) {
        for(vector<Vector2>::size_type i = 0; i != p.size(); i++) {
            p[i].x -= 1/fps * speed;
            p[i].y -= 1/fps * speed;
        }
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

vector<Vector2> Car::rotateCar(vector<Vector2> pCar) {
    float rad = degrees * PI / 180; // transforma graus para radianos
    Vector2 mid = (pCar[0] + pCar[1] + pCar[2]) / 3;
    for(vector<Vector2>::size_type i = 0; i != pCar.size(); i++)
        pCar[i] = Utils::rotatePoint(pCar[i], mid, rad);
    return pCar;
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
    p = rotateCar(p);
}

void Car::checkCoinCollision()
{
    for(vector<Vector2>::size_type i = 0; i != p.size(); i++) {
        if (Utils::checkCircleCollision(p[i].x, p[i].y, coinPos, COIN_RAD)) {
            coinCount++;
            isCoinOn = false;
        }
    }
}
