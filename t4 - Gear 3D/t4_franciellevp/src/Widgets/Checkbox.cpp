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
Checkbox::Checkbox (Gear** gear) : Widget()
{
    this->gearController = gear;
    create();
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
    this->p.x = x;
    this->p.y = y;
    this->width = w;
    this->height  = h;
    this->bgColor = bg;
}

/* Renderiza/desenha o checkbox na tela
*/
void Checkbox::render()
{
    CV::color(bgColor[0], bgColor[1], bgColor[2]);
    CV::rectFill(p.x, p.y, p.x + width, p.y + height);
    vector<float> c = Utils::RGBtoFloat(65,105,225);
    CV::color(c[0], c[1], c[2]);
    CV::circle(p.x + width/2, p.y + height/2, width/3, 100);
    CV::color(labelColor[0], labelColor[1], labelColor[2]);
    CV::text(p.x + width + 5, p.y + 5 + height/2, label.c_str());
    if (isChecked) {
        CV::color(c[0], c[1], c[2]);
        CV::circleFill(p.x + width/2, p.y + height/2, width/3, 100);
    }

    CV::text(975, 440, "Rotacoes:");
    CV::text(975, 510, "Visualizacao:");
}

/* Percorre um array contendo todos os checkboxes para desenha-los na tela
*/
void Checkbox::renderWidgets()
{
    for(vector<Checkbox>::size_type i = 0; i != check.size(); i++)
        check[i]->render();
}

/* Verifica a colisao com cada checkbox criado, realizando as operacoes necessarias quando colide
   @param state: verifica se botao do mouse foi clicado
   @param x: coordenada x do mouse
   @param y: coordenada y do mouse
*/
void Checkbox::checkState(int button, int state, int x, int y)
{
    for(vector<Checkbox>::size_type i = 0; i != check.size(); i++) {
        if( state == 0 ) {
            if(check[i]->checkCollision(x, y)) {
                check[i]->isChecked = (check[i]->isChecked) ? false : true;
                if (i == 0) (*gearController)->setRotateZ(check[0]->isChecked);
                if (i == 1) (*gearController)->setRotateY(check[1]->isChecked);
                if (i == 2) (*gearController)->setRotateX(check[2]->isChecked);

                if (i == 3) {
                    (*gearController)->orthographic = check[3]->isChecked;
                    (*gearController)->orthographicDraw();
                    uncheckOthers(3);
                }
                if (i == 4) {
                    (*gearController)->perspective = check[4]->isChecked;
                    (*gearController)->perspectiveDraw();
                    uncheckOthers(4);
                }
            }
        }
    }
}

/* Cria todos os botoes da aplicacao
*/
void Checkbox::create()
{
    vector<float> labelColor = Utils::RGBtoFloat(28,28,28);
    check.push_back(new Checkbox(975, 455, 20, 20, true, "Z", labelColor, Utils::RGBtoFloat(255,250,250)));
    check.push_back(new Checkbox(1035, 455, 20, 20, false, "Y", labelColor, Utils::RGBtoFloat(255,250,250)));
    check.push_back(new Checkbox(1095, 455, 20, 20, false, "X", labelColor, Utils::RGBtoFloat(255,250,250)));

    check.push_back(new Checkbox(975, 525, 20, 20, true, "Ortografica", labelColor, Utils::RGBtoFloat(255,250,250)));
    check.push_back(new Checkbox(975, 555, 20, 20, false, "Perspectiva", labelColor, Utils::RGBtoFloat(255,250,250)));
}

void Checkbox::uncheckOthers (unsigned int checked)
{
    if (checked > 2) {
        for(vector<Checkbox>::size_type i = 3; i < 5; i++) {
            if (i != checked)
                if (check[i]->isChecked)
                    check[i]->isChecked = false;
        }
    }
    (*gearController)->setRotateZ(check[0]->isChecked);
    (*gearController)->setRotateY(check[1]->isChecked);
    (*gearController)->setRotateX(check[2]->isChecked);

    (*gearController)->orthographic = check[3]->isChecked;
    (*gearController)->perspective = check[4]->isChecked;
}

void Checkbox::keyboardCheck(int key) {}

