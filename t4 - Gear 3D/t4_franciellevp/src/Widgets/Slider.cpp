//****************************************************************
//
// classe derivada de Widget que controla os sliders da aplicacao
//
//****************************************************************
#include "Slider.h"

#include "../Canvas/gl_canvas2d.h"
#include "../Utils/Utils.h"
#include <iostream>

#define RAD 8

using namespace std;

/* Inicia todos os atributos necessarios
   @param bc: instancia da classe Bezier para controlar atributos da pista com sliders
*/
Slider::Slider (Gear** gear) : Widget()
{
    this->canDrag = false;
    this->gearController = gear;
    create();
    //sliders[0]->currentValue = (*gearController)->width;
}

Slider::~Slider() {}

/* Inicia todos os atributos necessarios
   @param x: coordenada x do canto superior esquerdo que o alert comeca a ser desenhado
   @param y: coordenada y do canto superior esquerdo que o alert comeca a ser desenhado
   @param w: largura da janela
   @param h: altura da janela
   @param bg: array contendo a cor RGB do fundo do botao
   @param label: texto contido dentro do botao
   @param minRange: valor minimo do slide
   @param maxRange: valor maximo do slide
*/
Slider::Slider(int x, int y, float w, float h, vector<float> bg, const string label, int minRange, int maxRange)
    : Widget(), label(label), minRange(minRange), maxRange(maxRange)
{
    this->p.x = x;
    this->p.y = y;
    this->width = w;
    this->height  = h;
    this->bgColor = bg;
    this->circle = Vector2{p.x, p.y};
    this->canDrag = false;
    this->currentValue = this->minRange;
}

/* Retorna o valor atual do slider
*/
float Slider::getCurrentValue()
{
    return currentValue;
}

/* Renderiza/desenha o input na tela
*/
void Slider::render()
{
    CV::color(bgColor[0], bgColor[1], bgColor[2]);
    CV::rectFill(p.x, p.y, p.x + width, p.y + height);
    CV::color(bgColor[0], bgColor[1], bgColor[2]);
    CV::circleFill(circle.x, circle.y, RAD, 100);
    CV::text(p.x - RAD, p.y - 15, label.c_str());
    //CV::text(p.x - RAD, p.y - 15, currentValue.c_str()); mostrar o valor atual do slider
}

/* Percorre um array contendo todos os slider para desenha-los na tela
*/
void Slider::renderWidgets()

{
    for (vector<Slider>::size_type i = 0; i != sliders.size(); i++)
        sliders[i]->render();
}

/* Verifica a colisao com cada slider criado, realizando as operacoes necessarias quando colide
   @param button: verifica qual botao do mouse foi clicado
   @param state: verifica se botao do mouse foi clicado
   @param x: coordenada x do mouse
   @param y: coordenada y do mouse
*/
void Slider::checkState(int button, int state, int x, int y)
{
    for (vector<Slider>::size_type i = 0; i != sliders.size(); i++) {
        if (button == 0) {
            if (state == 0) {
                if (Utils::checkCircleCollision(x, y, sliders[i]->circle, RAD)) {
                        sliders[i]->canDrag = true;
                }
            } else if (state == 1) // soltou o mouse
                sliders[i]->canDrag = false;
        } else if (button == -2) {
            if (sliders[i]->canDrag) {
                Slider* s = sliders[i];
                if (x >= s->p.x && x <= s->p.x + s->width) {
                    sliders[i]->circle.x = x;
                    float percentPx = (s->circle.x - s->p.x) * 100 / s->width;
                    float valueSlided = percentPx * (s->maxRange - s->minRange) / 100;
                    s->currentValue = s->minRange + valueSlided;
                    if (i == 0)
                        (*gearController)->setWidth(s->currentValue);
                    else if (i == 1)
                        (*gearController)->setNroTeeth(s->currentValue);
                    else if (i == 2)
                        (*gearController)->setRadius(s->currentValue);
                    else if (i == 3)
                        (*gearController)->setRadiusBack(s->currentValue);
                    else if (i == 4)
                        (*gearController)->screenDist = s->currentValue;
                    else if (i == 5)
                        (*gearController)->velTranslation = s->currentValue;
                    else if (i == 6)
                        (*gearController)->velRotation = s->currentValue;
                }
            }
        }
    }
}

void Slider::create()
{
    vector<float> bg = Utils::RGBtoFloat(28,28,28);
    sliders.push_back(new Slider(980, 50, 120, 2, bg, "Espessura:", 1, 50));
    sliders.push_back(new Slider(980, 100, 120, 2, bg, "Nro de dentes:", 3, 20));
    sliders.push_back(new Slider(980, 150, 120, 2, bg, "Raio frontal:", 10, 50));
    sliders.push_back(new Slider(980, 200, 120, 2, bg, "Raio traseiro:", 10, 50));
    sliders.push_back(new Slider(980, 250, 120, 2, bg, "Distancia camera:", 20, 400));
    sliders.push_back(new Slider(980, 300, 120, 2, bg, "Velocidade translacao:", 0, 4));
    sliders.push_back(new Slider(980, 350, 120, 2, bg, "Velocidade rotacao:", 1, 10));
}

/* Cria todos os inputs da aplicacao
*/
void Slider::keyboardCheck(int key) {}

