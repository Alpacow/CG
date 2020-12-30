//*******************************************************************************
//
// classe para fazer o controle e comunicacao necessaria entre as demais classes
//
//*******************************************************************************
#include "Bezier.h"
#include "../Utils/Utils.h"
#include "../Canvas/gl_canvas2d.h"
#include <iostream>

#define INDEX 101
#define INDEX_POLY 33

using namespace std;

/* Inicia os atributos necessarios
*/
Bezier::Bezier()
{
    this->cp = new ControlPoints();
    canDrawSpeedWay = false;
    speedWayWidth = 30;
    estimatedPoints.reserve(INDEX);
    bezierPointsIn.reserve(INDEX_POLY);
    bezierPointsOut.reserve(INDEX_POLY);
    for(unsigned int i = 0; i < INDEX; i++)
        estimatedPoints.push_back(Vector2 {0, 0});
    for(unsigned int i = 0; i < INDEX_POLY; i++) {
        bezierPointsIn.push_back(Vector2 {0, 0});
        bezierPointsOut.push_back(Vector2 {0, 0});
    }
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
        // TODO: APPLY TRANSFORMATIONS
    }
    if (cp->points.size() > 1)
        drawBezierCurve(1);
    if (cp->points.size() > 2)
        drawBezierCurveForPolygon();
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
    if (cp->checkCollisionFirstPoint())
        canDrawSpeedWay = true;
    getPointsBezier();
}

void Bezier::getPointsBezier()
{
    int index = 0, counter = 0;
    for(float t = 0; t <= 1; t += 0.01) {
        float x = 0, y = 0;
        float lastP = cp->points.size() - 1;
        for(unsigned int i = 0; i < cp->points.size(); i++) {
            x += bernstein(lastP, i) * pow(1 - t, lastP - i) * pow(t, i) * cp->points[i]->getPoint().x;
            y += bernstein(lastP, i) * pow(1 - t, lastP - i) * pow(t, i) * cp->points[i]->getPoint().y;
        }
        estimatedPoints.at(index).x = x;
        estimatedPoints.at(index).y = y;
        index++;
        counter++;
    }
}

// TODO: IMPLEMENTAR TRANSFORMAÇÕES NA CURVA INTEIRA
void Bezier::drawBezierCurve(float maxValue)
{
    int idx = 0;
    for (unsigned int i = 1; i < estimatedPoints.size() - 1; i += 3) {
        Vector2 l = estimatedPoints[i - 1]; // left point
        Vector2 r = estimatedPoints[i + 1]; // center point
        Vector2 c = estimatedPoints[i]; // right point
        Vector2 v1 = c - l;
        Vector2 v2 = r - c;
        Vector2 n1 = v1.normalizeTo(speedWayWidth/2).getPerpendicular();
        Vector2 n2 = v2.normalizeTo(speedWayWidth/2).getPerpendicular();
        Vector2 p1a = l + n1; // primeiro vetor perpendicular
        Vector2 p1b = l - n1;
        Vector2 p2a = r + n2;
        Vector2 p2b = r - n2;
        Vector2 c1a = c + n1; //segundo vetor perpendicular
        Vector2 c1b = c - n1;
        Vector2 c2a = c + n2;
        Vector2 c2b = c - n2;
        Vector2 ca = Utils::intersecLines2d(p1a, c1a, p2a, c2a); // pontos centrais
        Vector2 cb = Utils::intersecLines2d(p1b, c1b, p2b, c2b);
        bezierPointsOut.at(idx) = cb;
        bezierPointsIn.at(idx) = ca;
        idx++;
        rgb = Utils::RGBtoFloat	(255,165,0);
        CV::color(rgb[0], rgb[1], rgb[2]);
        CV::line(estimatedPoints[i-1].x, estimatedPoints[i-1].y, estimatedPoints[i].x, estimatedPoints[i].y);
    }
}

void Bezier::drawBezierCurveForPolygon()
{
    rgb = Utils::RGBtoFloat(79,79,79);
    CV::color(rgb[0], rgb[1], rgb[2]);
    for (unsigned int i = 1; i < bezierPointsIn.size(); i++)
        CV::line(bezierPointsIn[i-1].x, bezierPointsIn[i-1].y, bezierPointsIn[i].x, bezierPointsIn[i].y);
    for (unsigned int i = 1; i < bezierPointsOut.size(); i++)
        CV::line(bezierPointsOut[i-1].x, bezierPointsOut[i-1].y, bezierPointsOut[i].x, bezierPointsOut[i].y);
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
