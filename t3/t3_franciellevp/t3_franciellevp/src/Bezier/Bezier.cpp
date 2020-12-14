
//*******************************************************************************
//
// classe para fazer o controle e comunicacao necessaria entre as demais classes
//
//*******************************************************************************
#include "Bezier.h"
#include "../Canvas/gl_canvas2d.h"
#include <iostream>

using namespace std;

/* Inicia os atributos necessarios
*/
Bezier::Bezier()
{
}

Bezier::~Bezier() {}

/* Renderiza/desenha tudo que sera necessario na tela
*/
void Bezier::render()
{
    for(vector<ControlPoints>::size_type i = 0; i != cp.size(); i++)
        cp[i]->render();
    drawControlGraph();
}

/* Controla a posicao do mouse e se houve clique ou nao
   @param button: estado dos botões do mouse
   @param x: coordenada x do mouse
   @param y: coordenada y do mouse
*/
void Bezier::checkMouseStates(int button, int x, int y)
{
    if (button == 0) { // clicou no botao esquerdo
        if(cp.size() > 0) cp[cp.size()-1]->setIsSelect(false); // ultimo ponto deixa de ser o selecionado
        cp.push_back(new ControlPoints(x, y, cp.size() + 1));
    }
}

void Bezier::drawControlGraph()
{
    CV::color(0, 0, 0);
    if (cp.size() > 1) {
        for (unsigned int i = 0; i < cp.size() - 1; i++) {
            Vector2 p1 = cp[i]->getPoint();
            Vector2 p2 = cp[i+1]->getPoint();
            CV::line(p1.x, p1.y, p2.x, p2.y);
        }
    }
}
