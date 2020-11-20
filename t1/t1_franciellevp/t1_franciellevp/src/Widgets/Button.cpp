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
    Create();
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
void Button::Render()
{
    CV::color(bgColor[0], bgColor[1], bgColor[2]);
    CV::rectFill(x, y, x + width, y + height);
    CV::color(labelColor[0], labelColor[1], labelColor[2]);
    CV::text(x+5, y+height/2, label.c_str()); //escreve o label do botao mais ou menos ao centro.
}

/* Percorre um array contendo todos os botoes para desenha-los na tela
*/
void Button::RenderWidgets()
{
    for(vector<Button>::size_type i = 0; i != bts.size(); i++)
        bts[i]->Render();
}

/* Verifica a colisao com cada botao criado, realizando as operacoes necessarias quando colide
   @param state: verifica se botao do mouse foi clicado
   @param x: coordenada x do mouse
   @param y: coordenada y do mouse
*/
void Button::CheckState(int state, int x, int y)
{
    if( state == 0 ) {
        if(bts[0]->Colidiu(x, y))
            (*imgController)->rotate90(TRUE);
        else if(bts[1]->Colidiu(x, y))
            (*imgController)->rotate90(FALSE);
        else if(bts[2]->Colidiu(x, y))
            (*imgController)->sepiaEffect();
        else if(bts[3]->Colidiu(x, y)) {
            (*imgController)->luminance();
            (*imgController)->channel[3] = 1;
        } else if(bts[4]->Colidiu(x, y))
            (*imgController)->blurEffect();
        else if(bts[5]->Colidiu(x, y))
            (*imgController)->scale();
        else if(bts[6]->Colidiu(x, y))
            (*imgController)->write();
    }
}

/* Cria todos os botoes da aplicacao
*/
void Button::Create()
{
    vector<float> bg = Utils::RGBtoFloat(255,250,250);
    vector<float> labelColor = Utils::RGBtoFloat(28, 28, 28);
    bts.push_back(new Button(880, 60, 100, 20, bg, "Horario", labelColor));
    bts.push_back(new Button(720, 60, 130, 20, bg, "Anti-horario", labelColor));

    bg = Utils::RGBtoFloat(210,180,140);
    bts.push_back(new Button(420, 100, 140, 20, bg, "Efeito Sepia", labelColor));
    bg = Utils::RGBtoFloat(169,169,169);
    bts.push_back(new Button(420, 130, 140, 20, bg, "Luminancia", labelColor));
    bg = Utils::RGBtoFloat(147,112,219);
    bts.push_back(new Button(420, 160, 140, 20, bg, "Borrar Imagem", labelColor));
    bg = Utils::RGBtoFloat(143,188,143);
    bts.push_back(new Button(820, 130, 210, 20, bg, "Diminuir pela metade", labelColor));
    bg = Utils::RGBtoFloat(250,128,114);
    bts.push_back(new Button(820, 160, 160, 20, bg, "Salvar imagem", labelColor));
}

