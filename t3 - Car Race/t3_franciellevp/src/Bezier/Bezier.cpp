//*******************************************************************************
//
// classe para fazer o controle e comunicacao necessaria entre as demais classes
//
//*******************************************************************************
#include "Bezier.h"
#include "../Utils/Utils.h"
#include "../Canvas/gl_canvas2d.h"
#include <iostream>

using namespace std;

/* Inicia os atributos necessarios
*/
Bezier::Bezier()
{
    this->cp = new ControlPoints();
    canDrawSpeedWay = false;
    speedWayWidth = 60;
}

Bezier::~Bezier() {}

void Bezier::setCanDragSpeedWay(bool value)
{
    canDrawSpeedWay = value;
}

/* Renderiza/desenha tudo que sera necessario na tela
*/
void Bezier::render()
{
    if (!canDrawSpeedWay) {
        for(vector<ControlPoints>::size_type i = 0; i != cp->points.size(); i++)
            cp->points[i]->render();
        cp->drawControlGraph();
        // CAN APPLY TRANSFORMATIONS
    }
    if (cp->points.size() > 1)
        drawBezierCurve(1);
}

/* Controla a posicao do mouse e se houve clique ou nao
   @param button: estado dos botões do mouse
   @param x: coordenada x do mouse
   @param y: coordenada y do mouse
*/
void Bezier::checkMouseStates(int button, int x, int y, int state)
{
    if (!canDrawSpeedWay) { // ainda nao finalizou a inserção de pontos de controle
        ControlPoints* cPoint = cp->checkCollisionDragPoints(x, y);
        if (button == 0) {
            if (state == 0) { // clicou no botao esquerdo
                if (cPoint == nullptr) // se nao colide com nenhum outro ponto
                    cp->addPoint(x, y);
                else {
                    cp->unsetAllPoints();
                    cPoint->canDragPoint = true;
                    cPoint->isSelect = true;
                }
            } else if (state == 1 && cPoint != nullptr)// soltou o mouse
                cPoint->canDragPoint = false;
        } else if(button == -2) {
            for (unsigned int i = 0; i < cp->points.size(); i++)
                if(cp->points[i]->canDragPoint && cp->points[i]->isSelect && cp->points[i]->checkControlPointArea(x, y))
                    cp->points[i]->dragSelectPoint(x, y);
        }
    }
    if (cp->checkCollisionFirstPoint()) { // TODO: parar de permitir pontos e desenhar pista
        canDrawSpeedWay = true;
    }
}

void Bezier::drawBezierCurve(float maxValue)
{
    float lastX = 0, lastY = 0;
    for(float t = 0; t <= maxValue; t += 0.01) {
        float x = 0, y = 0;
        float lastP = cp->points.size() - 1;
        for(unsigned int i = 0; i < cp->points.size(); i++) {
            x += bernstein(lastP, i) * pow(1 - t, lastP - i) * pow(t, i) * cp->points[i]->getPoint().x;
            y += bernstein(lastP, i) * pow(1 - t, lastP - i) * pow(t, i) * cp->points[i]->getPoint().y;
        }
        CV::color(1, 0, 0);
        if(t != 0) {
            CV::line(lastX, lastY, x, y);
            if (canDrawSpeedWay) {
                for(float i = 0; i < speedWayWidth / 2; i += 0.1) {
                    float lastX2 = lastX + i;
                    float x2 = x + i;
                    CV::line(lastX2, lastY, x2, y);
                    lastX2 = lastX - i;
                    x2 = x - i;
                    CV::line(lastX2, lastY, x2, y);
                    // TODO: tentar usar void CV::polygonFill(float vx[], float vy[], int elems)
                    // para desenhar a pista filled
                    // https://stackoverflow.com/questions/5783362/how-to-smoothly-vary-width-at-various-points-of-a-bezier-curve-drawn-using-gldra
                }
            }
        }
        lastX = x;
        lastY = y;
    }
}

ControlPoints* Bezier::getControlPoints()
{
    return cp;
}

double Bezier::bernstein(float n, float i)
{
    return (factorial(n) / (factorial(i) * factorial(n - i)));
}

double Bezier::factorial(int n)
{
    return (n == 0) ? 1 : n * factorial(n - 1);
}
