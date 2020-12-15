//****************************************************************
//
// classe derivada de Widget que controla os checkbox da aplicacao
//
//****************************************************************
#include "Checkbox.h"
#include "../Canvas/gl_canvas2d.h"
#include "../Utils/Utils.h"
#include <iostream>

using namespace std;

/* Inicia todos os atributos necessarios
   @param img: instancia da classe Bmp para controlar as acoes dos checkbox na imagem
*/
Checkbox::Checkbox (Bmp** img, Histogram** hist) : Widget()
{
    this->imgController = img;
    this->histController = hist;
    Create();
}

Checkbox::~Checkbox() {}

/* Inicia todos os atributos necessarios
   @param x: coordenada x do canto superior esquerdo que o alert comeca a ser desenhado
   @param y: coordenada y do canto superior esquerdo que o alert comeca a ser desenhado
   @param w: largura da janela
   @param h: altura da janela
   @param isChecked: 0 ou 1, indica se o checkbox esta com Check ou nao
   @param label: texto contido dentro do botao
   @param labelColor: array contendo a cor RGB do texto do botao
   @param bg: array contendo a cor RGB do fundo do botao
*/
Checkbox::Checkbox(int x, int y, float w, float h, int isChecked, string label, vector<float> labelColor, vector<float> bg)
    : Widget(), isChecked(isChecked), label(label), labelColor(labelColor)
{
    this->x = x;
    this->y = y;
    this->width = w;
    this->height  = h;
    this->bgColor = bg;
}

/* Renderiza/desenha o checkbox na tela
*/
void Checkbox::Render()
{
    CV::color(bgColor[0], bgColor[1], bgColor[2]);
    CV::rectFill(x, y, x + width, y + height);
    vector<float> c = Utils::RGBtoFloat(65,105,225);
    CV::color(c[0], c[1], c[2]);
    CV::circle(x + width/2, y + height/2, width/3, 100);
    CV::color(labelColor[0], labelColor[1], labelColor[2]);
    CV::text(x+width+5, y+5+height/2, label.c_str());
    if (isChecked) {
        CV::color(c[0], c[1], c[2]);
        CV::circleFill(x + width/2, y + height/2, width/3, 100);
    }
}

/* Percorre um array contendo todos os checkboxes para desenha-los na tela
*/
void Checkbox::RenderWidgets()
{
    for(vector<Checkbox>::size_type i = 0; i != check.size(); i++)
        check[i]->Render();
}

/* Verifica a colisao com cada checkbox criado, realizando as operacoes necessarias quando colide
   @param state: verifica se botao do mouse foi clicado
   @param x: coordenada x do mouse
   @param y: coordenada y do mouse
*/
void Checkbox::CheckState(int state, int x, int y)
{
    for(vector<Checkbox>::size_type i = 0; i != check.size(); i++) {
        if( state == 0 ) {
            if(check[i]->Colidiu(x, y)) {
                check[i]->isChecked = (check[i]->isChecked) ? FALSE : TRUE;
                if (i == 0) (*imgController)->mirrorH();
                else if (i == 1) (*imgController)->mirrorV();
                else if (i == 2 || i == 3 || i == 4) {
                    if (check[2]->isChecked || check[3]->isChecked || check[4]->isChecked)
                        (*histController)->isLuminance = 0;
                    if (!check[2]->isChecked && !check[3]->isChecked && !check[4]->isChecked) {
                        (*imgController)->channel = {1, 1, 1};
                        (*histController)->isLuminance = 1;
                    }
                    else {
                        (*imgController)->channel = {0, 0, 0};
                        if (check[2]->isChecked) (*imgController)->channel[0] = 1;
                        else (*imgController)->channel[0] = 0;
                        if (check[3]->isChecked) (*imgController)->channel[1] = 1;
                        else (*imgController)->channel[1] = 0;
                        if (check[4]->isChecked) (*imgController)->channel[2] = 1;
                        else (*imgController)->channel[2] = 0;
                    }
                }
            }
        }
    }
}

/* Cria todos os botoes da aplicacao
*/
void Checkbox::Create()
{
    vector<float> labelColor = Utils::RGBtoFloat(255, 250, 250);
    check.push_back(new Checkbox(420, 40, 20, 20, FALSE, "Espelhar Horizontalmente", labelColor, Utils::RGBtoFloat(255,250,250)));
    check.push_back(new Checkbox(420, 70, 20, 20, FALSE, "Espelhar Verticalmente", labelColor, Utils::RGBtoFloat(255,250,250)));
    // canais RGB
    labelColor = Utils::RGBtoFloat(255,99,71);
    check.push_back(new Checkbox(600, 130, 20, 20, FALSE, "RED", labelColor, Utils::RGBtoFloat(255,250,250)));
    labelColor = Utils::RGBtoFloat(50,205,50);
    check.push_back(new Checkbox(660, 130, 20, 20, FALSE, "GREEN", labelColor, Utils::RGBtoFloat(255,250,250)));
    labelColor = Utils::RGBtoFloat(30,144,255);
    check.push_back(new Checkbox(740, 130, 20, 20, FALSE, "BLUE", labelColor, Utils::RGBtoFloat(255,250,250)));
}

