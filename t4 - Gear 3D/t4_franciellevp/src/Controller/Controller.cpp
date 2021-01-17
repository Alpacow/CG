//*******************************************************************************
//
// classe para fazer o controle e comunicacao necessaria entre as demais classes
//
//*******************************************************************************
#include "Controller.h"
#include "../Widgets/Slider.h"
#include "../Widgets/Button.h"
#include "../Widgets/Checkbox.h"
#include "../Canvas/gl_canvas2d.h"
#include <iostream>

using namespace std;

/* Inicia os atributos necessarios
*/
Controller::Controller()
{
    screenWidth = 1200;
    screenHeight = 620;
    fps = 100.0;
    frames  = new Frames();
    alerts = new Alert();
    wds.push_back(alerts);
    wds.push_back(new Button());
    //wds.push_back(new Input());
    wds.push_back(new Slider());
}

Controller::~Controller() {}

/* Renderiza/desenha tudo que sera necessario na tela
*/
void Controller::Render()
{
    rgb = Utils::RGBtoFloat(220,220,220);
    CV::color(rgb[0], rgb[1], rgb[2]);
    CV::rectFill(screenWidth - 200, 0, screenWidth, screenHeight);

    for(vector<Widget>::size_type i = 0; i != wds.size(); i++)
        wds[i]->renderWidgets();
    fps = frames->getFrames();
    char* text = (char*)malloc(sizeof(char) * 30);
    sprintf(text, "FPS: %.0f", fps);
    CV::text(10, screenHeight - 10, text);
}

/* Controla as teclas apertadas durante a execucao
   @param key: inteiro equivalente a tecla apertada
*/
void Controller::Keyboard(int key)
{
    for(vector<Widget>::size_type i = 0; i != wds.size(); i++)
        wds[i]->keyboardCheck(key);
    opcao = key;
    switch(key) {
        case Utils::Esc:
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
        wds[i]->checkState(button, state, x, y);
    //cout << mx << " " << my << endl;
}

/* Inicia a canvas
*/
void Controller::InitCanvas() {
    CV::init(&screenWidth, &screenHeight, "T4 - Engrenagem 3D");
    rgb = Utils::RGBtoFloat(245,245,220);
    CV::clear(rgb[0], rgb[1], rgb[2]);
    CV::run();
}
