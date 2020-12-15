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
    this->pn = pn;
    this->p.x = x;
    this->p.y = y;
    this->label = (char*)malloc(sizeof(char) * 2);
    sprintf(label, "P%d", pn);
    this->isSelect = true;
}

ControlPoints::~ControlPoints() {}

/* Renderiza/desenha tudo que sera necessario na tela
*/
void ControlPoints::render()
{
    if(isSelect) {
        CV::color(1, 0, 0);
        CV::circle(p.x, p.y, R * 2, 20);
    } else
        CV::color(0, 0, 0);
    CV::text(p.x - 14 * strlen(label), p.y - R, label);
    CV::circleFill(p.x, p.y, R, 20);
    //drawDragPoints();
}

Vector2 ControlPoints::getPoint()
{
    return p;
}

void ControlPoints::setIsSelect(bool value)
{
    isSelect = value;
}

bool ControlPoints::checkCollision(int mx,int my, Vector2 p)
{
    return (mx >= (p.x - R) && mx <= (p.x + R) && my >= (p.y - R) && my <= (p.y + R));
}

void ControlPoints::drawDragPoints()
{
    rgb = Utils::RGBtoFloat(70,130,180);
    CV::color(rgb[0], rgb[1], rgb[2]);
    CV::circleFill(p.x - DRAG_DIST, p.y - DRAG_DIST, R, 20);
    CV::line(p.x, p.y, p.x - DRAG_DIST, p.y - DRAG_DIST);
}

void ControlPoints::dragPoint(int mx,int my)
{
    //cout << "drag point to " << mx << " " << my << endl;
}
