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

using namespace std;

/* Inicia os atributos necessarios
*/
Bezier::Bezier()
{
    this->cp = new ControlPoints();
    canDrawSpeedWay = false;
    speedWayWidth = 30;
    bezierPointsIn.reserve(INDEX);
    bezierPointsOut.reserve(INDEX);
    cpIn.reserve(INDEX);
    cpOut.reserve(INDEX);
    for(unsigned int i = 0; i < INDEX; i++) {
        bezierPointsIn.push_back(Vector2{0, 0});
        bezierPointsOut.push_back(Vector2{0, 0});
        cpIn.push_back(Vector2{0, 0});
        cpOut.push_back(Vector2{0, 0});
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
    if (cp->points.size() > 2)
        drawBezierCurveForPolygon();
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
    if (cp->checkCollisionFirstPoint()) {
        canDrawSpeedWay = true;
    }
}
// TODO: IMPLEMENTAR TRANSFORMAÇÕES NA CURVA INTEIRA
void Bezier::drawBezierCurve(float maxValue)
{
    float lastX = 0, lastY = 0;
    int idx = 0; // ixd ate 202
    for(float t = 0; t <= maxValue; t += 0.01) {
        float x = 0, y = 0;
        float lastP = cp->points.size() - 1;
        for(unsigned int i = 0; i < cp->points.size(); i++) {
            x += bernstein(lastP, i) * pow(1 - t, lastP - i) * pow(t, i) * cp->points[i]->getPoint().x;
            y += bernstein(lastP, i) * pow(1 - t, lastP - i) * pow(t, i) * cp->points[i]->getPoint().y;
        }
        CV::color(1, 0, 0);
        //bezierPointsIn.at(idx) = Vector2{x, y};
        //bezierPointsOut.at(idx) = Vector2{xx, yy};
        idx = (idx <= INDEX) ? idx + 1 : 0;
        if(t != 0) {
            CV::line(lastX, lastY, x, y);
            /*if (canDrawSpeedWay) {
                // http://brunoimbrizi.com/unbox/2015/03/offset-curve/
                //https://seant23.wordpress.com/2010/11/12/offset-bezier-curves/
                //CV::polygonFill(bezierPointsIn);
                //CV::color(0, 1, 0);
                //CV::polygonFill(bezierPointsOut);
                CV::color(1, 0.8, 0.42);
                for(float i = 0; i < speedWayWidth / 2; i += 0.1) {
                    float lastX2 = lastX + i;
                    float x2 = x + i;
                    CV::line(lastX2, lastY, x2, y);
                    lastX2 = lastX - i;
                    x2 = x - i;
                    CV::line(lastX2, lastY, x2, y);
                }
            }*/
        }
        lastX = x;
        lastY = y;
    }
}

void Bezier::drawBezierCurveForPolygon()
{
    int idx = 0;
    for (unsigned int p = 1; p < cp->points.size() - 1; p += 2) {
        Vector2 p1 = cp->points[p - 1]->getPoint();
        Vector2 p2 = cp->points[p + 1]->getPoint();
        Vector2 c = cp->points[p]->getPoint();
        Vector2 v1 = c - p1;
        Vector2 v2 = p2 - c;
        Vector2 n1 = v1.normalizeTo(speedWayWidth).getPerpendicular();
        Vector2 n2 = v2.normalizeTo(speedWayWidth).getPerpendicular();
        Vector2 p1a = p1 + n1; // primeiro vetor perpendicular
        Vector2 p1b = p1 - n1;
        Vector2 p2a = p2 + n2;
        Vector2 p2b = p2 - n2;
        Vector2 c1a = c + n1; //segundo vetor perpendicular
        Vector2 c1b = c - n1;
        Vector2 c2a = c + n2;
        Vector2 c2b = c - n2;
        Vector2 ca = Utils::intersecLines2d(p1a, c1a, p2a, c2a);
        Vector2 cb = Utils::intersecLines2d(p1b, c1b, p2b, c2b);
        cpOut.at(idx) = p1b;
        cpOut.at(idx + 1) = cb;
        cpOut.at(idx + 2) = p2b;
        cpIn.at(idx) = p1a;
        cpIn.at(idx + 1) = ca;
        cpIn.at(idx + 2) = p2a;
        //idx += 3;
        float lastX = 0, lastY = 0, lastX2 = 0, lastY2 = 0;
        for(float t = 0; t <= 1; t += 0.01) {
            float x = 0, y = 0, xx = 0, yy = 0;
            float lastP = 2;
            for(int i = 0; i < 3; i++) {
                //cout << "tamanho "  << idx << " " << cpIn[i].x << endl;
                x += bernstein(lastP, i) * pow(1 - t, lastP - i) * pow(t, i) * cpIn[i].x;
                y += bernstein(lastP, i) * pow(1 - t, lastP - i) * pow(t, i) * cpIn[i].y;
                /*xx += bernstein(lastP, i) * pow(1 - t, lastP - i) * pow(t, i) * cpOut[i].x;
                yy += bernstein(lastP, i) * pow(1 - t, lastP - i) * pow(t, i) * cpOut[i].y;*/
            }
            CV::color(1, 0, 1);
            if(t != 0) {
                CV::line(lastX, lastY, x, y);
                CV::line(lastX2, lastY2, xx, yy);
            }
            lastX = x;
            lastX2 = xx;
            lastY = y;
            lastY2 = yy;
        }
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
