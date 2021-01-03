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
Bezier::Bezier(Slider* slider)
{
    this->slider = slider;
    cp = new ControlPoints();
    canApplyTransformations = false;
    translationMode = false;
    scaleMode = false;
    speedWayWidth = 30;
    center = Vector2 {0, 0};
    lastPosition = Vector2 {0, 0};
    estimatedPoints.reserve(INDEX);
    for(unsigned int i = 0; i < INDEX; i++)
        estimatedPoints.push_back(Vector2 {0, 0});
    updateSpeedWaySize();
}

Bezier::~Bezier() {}

/* Renderiza/desenha tudo que sera necessario na tela
*/
void Bezier::render()
{
    if (!canApplyTransformations) {
        for(vector<ControlPoints>::size_type i = 0; i != cp->points.size(); i++)
            cp->points[i]->render();
        cp->drawControlGraph();
    }
    if (cp->points.size() > 2)
        drawBezierCurveForPolygon();
    if (cp->points.size() > 1)
        drawBezierCurve();
    if (canApplyTransformations) {
        if (translationMode || scaleMode) {
            CV::color(0,1,0);
            CV::circleFill(center.x, center.y, 5, 25);
        }
    }
}

/* Controla a posicao do mouse e se houve clique ou nao
   @param button: estado dos bot�es do mouse
   @param x: coordenada x do mouse
   @param y: coordenada y do mouse
*/
void Bezier::checkMouseStates(int button, int x, int y, int state)
{
    ControlPoints* cPoint = cp->checkCollisionDragPoints(x, y);
    if (button == 0) {
        if (state == 0) { // clicou no botao esquerdo
            if (cPoint == nullptr && !canApplyTransformations) // se nao colide com nenhum outro ponto
                cp->addPoint(x, y);
            else if (cPoint != nullptr) {
                cp->unsetAllPoints();
                cPoint->canDragPoint = true;
                cPoint->isSelect = true;
            }
        } else if (state == 1 && cPoint != nullptr)// soltou o mouse
            cPoint->canDragPoint = false;
    } else if(button == -2) {
        for (unsigned int i = 0; i < cp->points.size(); i++)
            if(cp->points[i]->isSelect && cp->points[i]->checkControlPointArea(x, y))
                cp->points[i]->dragSelectPoint(x, y);
    }
    speedWayWidth = slider->getCurrentValue();
    if (cp->checkCollisionFirstPoint())
        canApplyTransformations = true;
    getPointsBezier();
    if (canApplyTransformations) {
        if (translationMode)
            translate(x, y);
        else if (scaleMode) {
            /*center = getCenterPoint();
            Vector2 scale1 = {x - center.x, y - center.y};
            Vector2 scale2 = {lastPosition.x - center.x, lastPosition.y - center.y};
            float scale = scale1.length() / scale2.length();
            cout << "scale is " << scale << endl;*/
            rescaleCurve(Vector2 {0.02, 0.02});
            //lastPosition = Vector2 {(float)x, (float)y};
        }
    }
}

void Bezier::getPointsBezier()
{
    int index = 0;
    for(float t = 0; t <= 1; t += 0.01) {
        float x = 0, y = 0;
        float lastP = cp->points.size() - 1;
        for(unsigned int i = 0; i < cp->points.size(); i++) {
            x += bernstein(lastP, i) * pow(1 - t, lastP - i) * pow(t, i) * cp->points[i]->point.x;
            y += bernstein(lastP, i) * pow(1 - t, lastP - i) * pow(t, i) * cp->points[i]->point.y;
        }
        estimatedPoints.at(index).x = x;
        estimatedPoints.at(index).y = y;
        index++;
    }
}

void Bezier::drawBezierCurve()
{
    int idx = 0;
    updateSpeedWaySize();
    for (float j = speedWayWidth; j > 0; j--) {
        for (unsigned int i = 1; i < estimatedPoints.size() - 1; i += 3) {
            Vector2 l = estimatedPoints[i - 1]; // left point
            Vector2 r = estimatedPoints[i + 1]; // center point
            Vector2 c = estimatedPoints[i]; // right point
            Vector2 n1 = (c - l).normalizeTo(j/2).getPerpendicular();
            Vector2 n2 = (r - c).normalizeTo(j/2).getPerpendicular();
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
            rgb = Utils::RGBtoFloat(255,215,0);
            CV::color(rgb[0], rgb[1], rgb[2]);
            CV::line(estimatedPoints[i-1].x, estimatedPoints[i-1].y, estimatedPoints[i].x, estimatedPoints[i].y);
        }
    }
}

void Bezier::drawBezierCurveForPolygon()
{
    rgb = Utils::RGBtoFloat(169,169,169);
    CV::color(rgb[0], rgb[1], rgb[2]);
    unsigned int currentSize = (!canApplyTransformations) ? INDEX_POLY : bezierPointsIn.size();
    for (unsigned int i = 1; i < currentSize; i++) {
        CV::line(bezierPointsIn[i-1].x, bezierPointsIn[i-1].y, bezierPointsIn[i].x, bezierPointsIn[i].y);
        CV::line(bezierPointsOut[i-1].x, bezierPointsOut[i-1].y, bezierPointsOut[i].x, bezierPointsOut[i].y);
    }
}

Vector2 Bezier::getCenterPoint()
{
    if (cp->points.size() <= 2)
        return (cp->points[0]->point + cp->points[1]->point) / 2;
    float cx = 0, cy = 0, det = 0, aux = 0;
    int j = 0;
    for (unsigned int i = 0; i < cp->points.size(); i++) {
        if (i + 1 == cp->points.size()) j = 0;
        else j = i + 1;
        Vector2 p = cp->points[i]->point;
        Vector2 p2 = cp->points[j]->point;
        aux = p.x * p2.y - p2.x * p.y;
        det += aux;
        cx += (p.x + p2.x) * aux;
        cy += (p.y + p2.y) * aux;
    }
    cx /= 3 * det;
    cy /= 3 * det;
    return Vector2 {cx, cy};
}

void Bezier::rescaleCurve(Vector2 scale) {
    for (unsigned int i = 0; i < cp->points.size(); i++) {
        float x = cp->points[i]->point.x * scale.x;
        float y = cp->points[i]->point.y * scale.y;
        cp->points[i]->point.x = x;
        cp->points[i]->point.y = y;
    }
}

void Bezier::rotateCurve(float rad) {
    center = getCenterPoint();
    for (unsigned int i = 0; i < cp->points.size(); i++)
        cp->points[i]->point = Utils::rotatePoint(cp->points[i]->point, center, rad);
}

void Bezier::translate(int x, int y) {
    Vector2 c = Vector2 {(float)x, (float)y};
    center = getCenterPoint();
    Vector2 diff = c - center;
    for (unsigned int i = 0; i < cp->points.size(); i++)
        cp->points[i]->point += diff;
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

void Bezier::updateSpeedWaySize()
{
    float reserved = INDEX_POLY * speedWayWidth;
    bezierPointsIn.resize(reserved);
    bezierPointsOut.resize(reserved);
    for(unsigned int i = 0; i < reserved; i++) {
        bezierPointsIn.at(i) = Vector2 {0, 0};
        bezierPointsOut.at(i) = Vector2 {0, 0};
    }
}
