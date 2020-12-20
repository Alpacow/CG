//*******************************************************************************
//
// classe para fazer o controle e comunicacao necessaria entre as demais classes
//
//*******************************************************************************
#include "ControlPoints.h"
#include "../Utils/Utils.h"
#include <iostream>

using namespace std;

/* Inicia os atributos necessarios
*/
ControlPoints::ControlPoints(int x, int y, int pn)
{
    this->nPoint = pn;
    this->point.x = x;
    this->point.y = y;
    this->dragPoint.x = x - DRAG_DIST;
    this->dragPoint.y = y - DRAG_DIST;
    this->label = (char*)malloc(sizeof(char) * 2);
    sprintf(label, "P%d", nPoint);
}

ControlPoints::ControlPoints() {}

ControlPoints::~ControlPoints() {}

void ControlPoints::setCanDragPoint(bool value)
{
    canDragPoint = value;
}

bool ControlPoints::getCanDragPoint()
{
    return canDragPoint;
}

Vector2 ControlPoints::getPoint()
{
    return point;
}

Vector2 ControlPoints::getDragPoint()
{
    return dragPoint;
}

Vector2 ControlPoints::getFirstPoint()
{
    return fstPoint;
}

/* Renderiza/desenha tudo que sera necessario na tela
*/
void ControlPoints::render()
{
    CV::color(0, 0, 0);
    CV::text(point.x - 14 * strlen(label), point.y - R, label);
    CV::circleFill(point.x, point.y, R, 20);
    drawDragPoints();
}

void ControlPoints::drawDragPoints()
{
    vector<float> rgb = Utils::RGBtoFloat(70,130,180);
    CV::color(rgb[0], rgb[1], rgb[2]);
    CV::circleFill(dragPoint.x, dragPoint.y, 2*R, 20);
    CV::line(point.x, point.y, dragPoint.x, dragPoint.y);
}

ControlPoints* ControlPoints::checkCollisionAll(int x, int y)
{
    for(vector<ControlPoints>::size_type i = 0; i != points.size(); i++)
        if (i != 0 && Utils::checkCircleCollision(x, y, points[i]->dragPoint, R))
            return points[i];
    return nullptr;
}

bool ControlPoints::checkControlPointArea(int x, int y)
{
    return (y > DRAW_Y_VALID && x < DRAW_X_VALID);
}

void ControlPoints::addPoint(int x, int y)
{
    if (checkControlPointArea(x, y)) {
        ControlPoints* p = new ControlPoints(x, y, points.size() + 1);
        points.push_back(p);
        if (points.size() == 1)
            fstPoint = p->point; // guarda ponteiro do primeiro ponto para fechar a pista depois
    }
}

void ControlPoints::drawControlGraph()
{
    CV::color(0, 0, 0);
    if (points.size() > 1) {
        for (unsigned int i = 0; i < points.size() - 1; i++) {
            Vector2 p1 = points[i]->getPoint();
            Vector2 p2 = points[i+1]->getPoint();
            CV::line(p1.x, p1.y, p2.x, p2.y);
        }
    }
}

void ControlPoints::clearControlPoints()
{
    points.clear();
}

void ControlPoints::dragSelectPoint(int mx, int my)
{
    if (canDragPoint) {
        dragPoint.x = mx;
        dragPoint.y = my;
        point.x = mx + DRAG_DIST;
        point.y = my + DRAG_DIST;
    }
}
