
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
void Bezier::checkMouseStates(int button, int x, int y, int state)
{
    if (button == 0 && state == 0) { // clicou no botao esquerdo
        if (cp.size() > 0) cp[cp.size()-1]->setIsSelect(false); // ultimo ponto deixa de ser o selecionado
        if (!checkCollisionAll(x, y)) { // se nao colide com nenhum outro ponto
            ControlPoints* p = new ControlPoints(x, y, cp.size() + 1);
            cp.push_back(p);
            if (cp.size() == 1) fstPoint = p; // guarda pos do primeiro ponto para fechar a pista
        }
    }
    cout << button << state << endl;
    for(vector<ControlPoints>::size_type i = 0; i != cp.size(); i++) {
        if (button == 0 && state == 0) {
            if (i != 0 && cp[i]->checkCollision(x, y, fstPoint->getPoint())) {
                cout << "finalizar pista" << endl; // TODO: parar de permitir pontos e desenhar pista
                return;
            }
            if (cp[i]->checkCollision(x, y, cp[i]->getPoint())) { // se colidir com outro ponto
                unsetAllPoints();
                cp[i]->setIsSelect(true);
                cp[i]->dragPoint(x, y);
            }
        }
        /*Vector2 pDrag = cp[i]->getPoint();
        pDrag.x -= DRAG_DIST;
        pDrag.y -= DRAG_DIST;
        if (cp[i]->checkCollision(x, y, pDrag) && state != -2) // se colidir com o botao de mover o ponto
            cp[i]->dragPoint(x, y);*/
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

bool Bezier::checkCollisionAll(int x, int y)
{
    for(vector<ControlPoints>::size_type i = 0; i != cp.size(); i++)
        if (i != 0 && cp[i]->checkCollision(x, y, cp[i]->getPoint()))
            return true;
    return false;
}

void Bezier::unsetAllPoints()
{
     for(vector<ControlPoints>::size_type i = 0; i != cp.size(); i++)
        cp[i]->setIsSelect(false);
}
