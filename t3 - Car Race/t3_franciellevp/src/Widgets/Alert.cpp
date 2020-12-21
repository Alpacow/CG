//******************************************************************************
//
// classe derivada de Widget que controla as janelas de mensagem da aplicacao
//
//******************************************************************************
#include "Alert.h"
#include "../Canvas/gl_canvas2d.h"
#include "../Utils/Utils.h"
#include <iostream>

using namespace std;

Alert::Alert () : Widget() {}

Alert::~Alert() {}

void Alert::create() {}

/* Inicia todos os atributos necessarios
   @param x: coordenada x do canto superior esquerdo que o alert comeca a ser desenhado
   @param y: coordenada y do canto superior esquerdo que o alert comeca a ser desenhado
   @param w: largura da janela
   @param h: altura da janela
   @param txt: texto da mensagem que deseja-se exibir
   @param type: tipo de alert (aviso, informativo ou erro)
   @param isActive: 1 -> janela aparece na tela, 0 -> janela nao eh desenhada na tela
*/
Alert::Alert(int x, int y, float w, float h, const string txt, int type, int isActive)
    : Widget(), txt(txt), type(type), isActive(isActive)
{
    this->p.x = x;
    this->p.y = y;
    this->width = w;
    this->height  = h;
    if (type == Utils::WARNING)
        this->bgColor = Utils::RGBtoFloat(255,215,0);
    else if (type == Utils::ERRO)
        this->bgColor = Utils::RGBtoFloat(255,99,71);
    else
        this->bgColor = Utils::RGBtoFloat(176,196,222);
}

/* Renderiza/desenha a janela e um botao "OK" para fechar a janela
*/
void Alert::render()
{
    CV::color(bgColor[0], bgColor[1], bgColor[2]);
    CV::rectFill(p.x, p.y, p.x + width, p.y + height);
    vector<float> color = Utils::RGBtoFloat(28, 28, 28);;
    CV::color(color[0], color[1], color[2]);
    int c = 5 * txt.size();
    CV::text(p.x - c + width/2, p.y + height/2, txt.c_str()); //escreve o label do botao mais ou menos ao centro.

    vector<float> bg = Utils::RGBtoFloat(128,128,128);
    color = Utils::RGBtoFloat(255, 250, 250);
    CV::color(bg[0], bg[1], bg[2]);
    int xb = p.x + width/2 - wb/2;
    int yb = p.y + height/1.5;
    CV::rectFill(xb, yb, xb+wb, yb+hb);
    CV::color(color[0], color[1], color[2]);
    CV::text(xb+18, yb+15, "OK");
}

/* Percorre um array contendo todos os alertas criados para desenha-los na tela
*/
void Alert::renderWidgets()
{
    for(vector<Alert>::size_type i = 0; i != alerts.size(); i++)
        if (alerts[i]->isActive)
            alerts[i]->render();
}

/* Verifica a colisao com cada alert criado, realizando as operacoes necessarias quando colide
   @param state: verifica se botao do mouse foi clicado
   @param x: coordenada x do mouse
   @param y: coordenada y do mouse
*/
void Alert::checkState(int state, int x, int y)
{
    if(state == 0) {
        for(vector<Alert>::size_type i = 0; i != alerts.size(); i++){
            if(alerts[i]->checkCollision(x, y)) {
                // se um aviso do tipo "ERRO" eh liberado, toda a aplicacao eh fechada
                if (alerts[i]->type ==  Utils::ERRO) exit(-1);
                alerts[i]->isActive = FALSE;
            }
        }
    }
}

/* Testa se colidiu com o botao dentro do alert
   Esse metodo foi reescrito pois deve-se testar a colisao com o botao da janela
   nao com a janela em si
   @param mx: coordenada x do mouse
   @param my: coordenada y do mouse
*/
bool Alert::checkCollision(int mx, int my)
{
    wb = 50;
    hb = 30;
    int xb = p.x + width / 2 - wb/2;
    int yb = p.y + height / 1.5;
    if(mx >= xb && mx <= (xb + wb) && my >= p.y && my <= (yb + hb))
        return true;
    return false;
}
