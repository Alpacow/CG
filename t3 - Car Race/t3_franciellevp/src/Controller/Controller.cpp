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
    screenHeight = 620;
    fps = 100.0;
    frames  = new Frames();
    alerts = new Alert();
    car = new Car(&alerts);
    bezier = new Bezier();
    wds.push_back(alerts);
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

    rgb = Utils::RGBtoFloat(211,211,211);
    CV::color(rgb[0], rgb[1], rgb[2]);
    CV::rectFill(0, 30, screenWidth, 60);

    rgb = Utils::RGBtoFloat(220,220,220);
    CV::color(rgb[0], rgb[1], rgb[2]);
    CV::rectFill(1030, 60, screenWidth, screenHeight);

    rgb = Utils::RGBtoFloat(28,28,28);
    CV::color(rgb[0], rgb[1], rgb[2]);
    CV::text(1040, 150, "Cor do carrinho:");
    for(vector<Widget>::size_type i = 0; i != wds.size(); i++)
        wds[i]->renderWidgets();
    fps = frames->getFrames();
    char* text = (char*)malloc(sizeof(char) * 30);
    sprintf(text, "FPS: %.0f", fps);
    CV::text(10, screenHeight - 10, text);
    bezier->render();
    car->render(fps);
}

/* Controla as teclas apertadas durante a execucao
   @param key: inteiro equivalente a tecla apertada
*/
void Controller::Keyboard(int key)
{
    //cout << "Tecla: " << key << endl;
    opcao = key;
    switch(key) {
        case Utils::Esc:
            exit(0);
            break;
        case Utils::Delete:
            bezier->getControlPoints()->clearControlPoints();
            break;
        case Utils::RightArrow:
            car->checkRotation(car->RightArrow);
            car->rotateCar();
            break;
        case Utils::LeftArrow:
            car->checkRotation(car->LeftArrow);
            car->rotateCar();
            break;
        case Utils::DownArrow:
            car->checkRotation(car->DownArrow);
            car->rotateCar();
            break;
        case Utils::UpArrow:
            car->checkRotation(car->UpArrow);
            car->rotateCar();
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
        wds[i]->checkState(state, x, y);
    //cout << button << state << endl;
    //cout << mx << " " << my << endl;
    bezier->checkMouseStates(button, x, y, state);
}

/* Inicia a canvas
*/
void Controller::InitCanvas() {
    CV::init(&screenWidth, &screenHeight, "T3 - Corridinha de Carrinho");
    rgb = Utils::RGBtoFloat(245,245,220);
    CV::clear(rgb[0], rgb[1], rgb[2]);
    CV::run();
}
