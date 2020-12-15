//****************************************************************
//
// classe derivada de Widget que controla os botoes da aplicacao
//
//****************************************************************
#include "Button.h"
#include "../Canvas/gl_canvas2d.h"
#include "../Utils/Utils.h"
#include <iostream>

using namespace std;

/* Inicia todos os atributos necessarios
   @param img: instancia da classe Bmp para controlar as acoes dos botoes na imagem
*/
Button::Button (Bmp** img) : Widget()
{
    this->imgController = img;
    create();
}

Button::~Button() {}

/* Inicia todos os atributos necessarios
   @param x: coordenada x do canto superior esquerdo que o alert comeca a ser desenhado
   @param y: coordenada y do canto superior esquerdo que o alert comeca a ser desenhado
   @param w: largura da janela
   @param h: altura da janela
   @param bg: array contendo a cor RGB do fundo do botao
   @param label: texto contido dentro do botao
   @param labelColor: array contendo a cor RGB do texto do botao
*/
Button::Button(int x, int y, float w, float h, vector<float> bg, const string label, vector<float> labelColor)
    : Widget(), label(label), labelColor(labelColor)
{
    this->x = x;
    this->y = y;
    this->width = w;
    this->height  = h;
    this->bgColor = bg;
}

/* Renderiza/desenha o botao na tela
*/
void Button::render()
{
    CV::color(bgColor[0], bgColor[1], bgColor[2]);
    CV::rectFill(x, y, x + width, y + height);
    CV::color(labelColor[0], labelColor[1], labelColor[2]);
    CV::text(x+5, y+height/2, label.c_str()); //escreve o label do botao mais ou menos ao centro.
}

/* Percorre um array contendo todos os botoes para desenha-los na tela
*/
void Button::renderWidgets()
{
    for(vector<Button>::size_type i = 0; i != bts.size(); i++)
        bts[i]->render();
}

/* Verifica a colisao com cada botao criado, realizando as operacoes necessarias quando colide
   @param state: verifica se botao do mouse foi clicado
   @param x: coordenada x do mouse
   @param y: coordenada y do mouse
*/
void Button::checkState(int state, int x, int y)
{
    if( state == 0 ) {
        if(bts[0]->checkCollision(x, y))
            (*imgController)->rotate90(TRUE);
        else if(bts[1]->checkCollision(x, y))
            (*imgController)->rotate90(FALSE);
    }
}

/* Cria todos os botoes da aplicacao
*/
void Button::create()
{
    vector<float> bg = Utils::RGBtoFloat(176,196,222);
    vector<float> labelColor = Utils::RGBtoFloat(28, 28, 28);
    bts.push_back(new Button(0, 30, 130, 30, bg, "Editar pista", labelColor));
    bts.push_back(new Button(132, 30, 160, 30, bg, "Iniciar corrida", labelColor));
}

