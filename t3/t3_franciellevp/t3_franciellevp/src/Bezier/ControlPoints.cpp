//*******************************************************************************
//
// classe para fazer o controle e comunicacao necessaria entre as demais classes
//
//*******************************************************************************
#include "ControlPoints.h"
#include <iostream>

using namespace std;

/* Inicia os atributos necessarios
*/
ControlPoints::ControlPoints(int x, int y, int pn)
{
    this->pn = pn;
    this->p.x = x;
    this->p.y = y;
    this->label = (char*)malloc(sizeof(char) * 5); // TODO: mudar isso aq para algo melhor
    sprintf(label, "P%d", pn);
    this->isSelect = true;
}

ControlPoints::~ControlPoints() {}

/* Renderiza/desenha tudo que sera necessario na tela
*/
void ControlPoints::render()
{
    CV::color(0, 0, 0);
    CV::text(p.x - 14 * strlen(label), p.y - R, label);
    CV::circleFill(p.x, p.y, R, 20);
    if(isSelect) CV::circle(p.x, p.y, R * 2, 20);
}

Vector2 ControlPoints::getPoint()
{
    return p;
}

void ControlPoints::setIsSelect(bool value)
{
    isSelect = value;
}
