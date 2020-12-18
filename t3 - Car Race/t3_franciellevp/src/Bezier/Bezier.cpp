
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
}

Bezier::~Bezier() {}

/* Renderiza/desenha tudo que sera necessario na tela
*/
void Bezier::render()
{
    for(vector<ControlPoints>::size_type i = 0; i != cp->points.size(); i++)
        cp->points[i]->render();
    cp->drawControlGraph();
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
    if (button == 0 && state == 0) { // clicou no botao esquerdo
        int pPosition = cp->checkCollisionAll(x, y);
        if (pPosition < 0) { // se nao colide com nenhum outro ponto
            cp->addPoint(x, y);
        } else {
            cp->unsetAllPoints();
            cp->points[pPosition]->setIsSelect(true);
            cp->setCanDragPoint(true);
        }
        if (cp->points.size() > 1 && Utils::checkCircleCollision(x, y, cp->getFirstPoint(), R)) {
            cout << "finalizar pista" << endl; // TODO: parar de permitir pontos e desenhar pista
            return;
        }
    } else if (button == 0 && state == 1) { // soltou o mouse
        cp->setCanDragPoint(true);
    }
    //cout << "AQ " << button << state << endl;
    /*for(vector<ControlPoints>::size_type i = 0; i != cp.size(); i++) {
        if (button == 0 && state == 0) {
            if (Utils::checkCircleCollision(x, y, cp[i]->getPoint(), R)) { // se colidir com outro ponto
                unsetAllPoints();
                cp[i]->setIsSelect(true);
                cp[i]->dragPoint(x, y);
            }
        }
        Vector2 pDrag = cp[i]->getPoint();
        pDrag.x -= DRAG_DIST;
        pDrag.y -= DRAG_DIST;
        if (cp[i]->checkCollision(x, y, pDrag) && state != -2) // se colidir com o botao de mover o ponto
            cp[i]->dragPoint(x, y);
    }*/
}

void Bezier::drawBezierCurve(float maxValue)
{
    double lastX = 0, lastY = 0;
    for(float t = 0; t <= maxValue; t += 0.01) {
        double x = 0, y = 0;
        double lastP = cp->points.size() - 1;
        for(unsigned int i = 0; i < cp->points.size(); i++) {
            x += bernstein(lastP, i) * pow(1 - t, lastP - i) * pow(t, i) * cp->points[i]->getPoint().x;
            y += bernstein(lastP, i) * pow(1 - t, lastP - i) * pow(t, i) * cp->points[i]->getPoint().y;
        }
        CV::color(1, 0, 0);
        if(t != 0)
            CV::line(lastX, lastY, x, y);
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
