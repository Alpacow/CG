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
    this->label = (char*)malloc(sizeof(char) * 2);
    sprintf(label, "P%d", nPoint);
    this->isSelect = true;
}

ControlPoints::ControlPoints() {}

ControlPoints::~ControlPoints() {}

/* Renderiza/desenha tudo que sera necessario na tela
*/
void ControlPoints::render()
{
    if(isSelect) {
        CV::color(1, 0, 0);
        CV::circle(point.x, point.y, R * 2, 20);
    } else
        CV::color(0, 0, 0);
    CV::text(point.x - 14 * strlen(label), point.y - R, label);
    CV::circleFill(point.x, point.y, R, 20);
    //drawDragPoints();
}

Vector2 ControlPoints::getPoint()
{
    return point;
}

Vector2 ControlPoints::getFirstPoint()
{
    return fstPoint;
}

void ControlPoints::setIsSelect(bool value)
{
    isSelect = value;
}

void ControlPoints::drawDragPoints()
{
    vector<float> rgb = Utils::RGBtoFloat(70,130,180);
    CV::color(rgb[0], rgb[1], rgb[2]);
    CV::circleFill(point.x - DRAG_DIST, point.y - DRAG_DIST, R, 20);
    CV::line(point.x, point.y, point.x - DRAG_DIST, point.y - DRAG_DIST);
}

void ControlPoints::dragPoint(int mx, int my)
{
    if (canDragPoint) {
        cout << "drag point" << endl;
        point.x = mx;
        point.y = my;
    }
}

int ControlPoints::checkCollisionAll(int x, int y)
{
    for(vector<ControlPoints>::size_type i = 0; i != points.size(); i++)
        if (i != 0 && Utils::checkCircleCollision(x, y, points[i]->point, R))
            return i;
    return -1;
}

void ControlPoints::addPoint(int x, int y)
{
    if (y > DRAW_Y_VALID && x < DRAW_X_VALID) {
        if (points.size() > 0) points[points.size()-1]->setIsSelect(false); // ultimo ponto deixa de ser o selecionado
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

void ControlPoints::unsetAllPoints()
{
     for(vector<ControlPoints>::size_type i = 0; i != points.size(); i++)
        points[i]->setIsSelect(false);
}

void ControlPoints::clearControlPoints()
{
    points.clear();
}

void ControlPoints::setCanDragPoint(bool value)
{
    canDragPoint = value;
}
