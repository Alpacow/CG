//****************************************************************
//
// classe derivada de Widget que controla os botoes da aplicacao
//
//****************************************************************
#include "Button.h"
#include "../Canvas/gl_canvas2d.h"
#include "../Utils/Utils.h"

using namespace std;

/* Inicia todos os atributos necessarios
   @param img: instancia da classe Bmp para controlar as acoes dos botoes na imagem
*/
Button::Button () : Widget()
{
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
    this->p.x = x;
    this->p.y = y;
    this->width = w;
    this->height  = h;
    this->bgColor = bg;
}

/* Renderiza/desenha o botao na tela
*/
void Button::render()
{
    CV::color(bgColor[0], bgColor[1], bgColor[2]);
    CV::rectFill(p.x, p.y, p.x + width, p.y + height);
    CV::color(labelColor[0], labelColor[1], labelColor[2]);
    CV::text(p.x + 5, p.y + height/2, label.c_str()); //escreve o label do botao mais ou menos ao centro.s
}

/* Percorre um array contendo todos os botoes para desenha-los na tela
*/
void Button::renderWidgets()
{
    bts[0]->render();
    for(vector<Button>::size_type i = 1; i != bts.size(); i++) {
        bts[i]->render();
    }
}

/* Verifica a colisao com cada botao criado, realizando as operacoes necessarias quando colide
   @param state: verifica se botao do mouse foi clicado
   @param x: coordenada x do mouse
   @param y: coordenada y do mouse
*/
void Button::checkState(int button, int state, int x, int y)
{
    if( state == 0 ) {
        //if (bts[0]->checkCollision(x, y))
            //(*bezierController)->showHelp = ((*bezierController)->showHelp) ? false : true;
    }
}

/* Cria todos os botoes da aplicacao
*/
void Button::create()
{
    vector<float> bg = Utils::RGBtoFloat(176,196,222);
    vector<float> labelColor = Utils::RGBtoFloat(28, 28, 28);
    bts.push_back(new Button(1060, 10, 80, 30, bg, "Ajuda", labelColor));
}

void Button::keyboardCheck(int key) {}

