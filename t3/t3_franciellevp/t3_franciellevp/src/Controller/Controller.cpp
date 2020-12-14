//*******************************************************************************
//
// classe para fazer o controle e comunicacao necessaria entre as demais classes
//
//*******************************************************************************
#include "Controller.h"
#include "../Canvas/gl_canvas2d.h"
#include "../Widgets/Button.h"
#include "../Widgets/Checkbox.h"
#include <iostream>

using namespace std;

/* Inicia os atributos necessarios
*/
Controller::Controller()
{
    screenWidth = 1200;
    screenHeight = 1000;
    alerts = new Alert();
    wds.push_back(alerts);
    wds.push_back(new Button(&img));
    wds.push_back(new Checkbox(&img));
}

Controller::~Controller() {}

/* Renderiza/desenha tudo que sera necessario na tela
*/
void Controller::Render()
{
    rgb = Utils::RGBtoFloat(28,28,28);
    CV::color(rgb[0], rgb[1], rgb[2]);
    CV::rectFill(0, 0, screenWidth, 30);
    rgb = Utils::RGBtoFloat(255,250,250);
    CV::color(rgb[0], rgb[1], rgb[2]);
    CV::text(screenWidth/2 - 180, 20, "T3 - Francielle Vasconcellos Pereira");
    //img->renderBmp(10, 40);
    for(vector<Widget>::size_type i = 0; i != wds.size(); i++)
        wds[i]->RenderWidgets();
    bezier->render();
}

/* Controla as teclas apertadas durante a execucao
   @param key: inteiro equivalente a tecla apertada
*/
void Controller::Keyboard(int key)
{
    cout << "Tecla: " << key << endl;
    opcao = key;
    switch(key) {
        case 27:
            exit(0);
            break;
    }
}

/* Controla a posicao do mouse e se houve clique ou nao
   @param x: coordenada x do mouse
   @param y: coordenada y do mouse
   @param state: estado do mouse (clicado ou nao)
*/
void Controller::Mouse(int button, int x, int y, int state)
{
    mx = x; //guarda as coordenadas do mouse para exibir dentro da render()
    my = y;
    for(vector<Widget>::size_type i = 0; i != wds.size(); i++)
        wds[i]->CheckState(state, x, y);
    if (state) {
        bezier->checkMouseStates(button, x, y);
    }
}

/* Inicia a canvas
*/
void Controller::InitCanvas() {
    CV::init(&screenWidth, &screenHeight, "T3 - Corridinha de Carrinho");
    img = new Bmp(Utils::getImagePath("car.bmp"), &alerts);
    bezier = new Bezier();
    rgb = Utils::RGBtoFloat(245,245,220);
    CV::clear(rgb[0], rgb[1], rgb[2]);
    CV::run();
}
