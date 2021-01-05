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
    bezier = new Bezier(&alerts);
    car = new Car(&alerts, &bezier);
    wds.push_back(alerts);
    wds.push_back(new Button(&bezier, &car));
    wds.push_back(new Input(&car));
    wds.push_back(new Slider(&bezier));
}

Controller::~Controller() {}

/* Renderiza/desenha tudo que sera necessario na tela
*/
void Controller::Render()
{
    rgb = Utils::RGBtoFloat(211,211,211);
    CV::color(rgb[0], rgb[1], rgb[2]);
    CV::rectFill(0, 0, screenWidth, 30);
    rgb = Utils::RGBtoFloat(220,220,220);
    CV::color(rgb[0], rgb[1], rgb[2]);
    CV::rectFill(screenWidth - 180, 30, screenWidth, screenHeight);
    bezier->render();

    rgb = Utils::RGBtoFloat(28,28,28);
    CV::color(rgb[0], rgb[1], rgb[2]);
    if (!bezier->raceOn)
        CV::text(1030, 290, "Rotacionar:");
    CV::text(screenWidth - 170, 150, "Cor do carrinho:");
    car->render(fps);

    for(vector<Widget>::size_type i = 0; i != wds.size(); i++)
        wds[i]->renderWidgets();
    fps = frames->getFrames();
    char* text = (char*)malloc(sizeof(char) * 30);
    sprintf(text, "FPS: %.0f", fps);
    CV::text(10, screenHeight - 10, text);

    if (bezier->showHelp) {
        rgb = Utils::RGBtoFloat(28,28,28);
        CV::color(rgb[0], rgb[1], rgb[2]);
        CV::rectFill(100, 100, 900, 600);
        rgb = Utils::RGBtoFloat(211,211,211);
        CV::color(rgb[0], rgb[1], rgb[2]);
        CV::text(440, 140, "MENU DE AJUDA");
        CV::text(120, 160, "1. Botao esquerdo adiciona pontos de controle e pontos de arrasto");
        CV::text(120, 180, "2. DELETE: limpa todos os pontos de controle/deleta a pista");
        CV::text(120, 220, "TRANSFORMACOES PODEM SER APLICADAS APENAS APOS O TERMINO DA PISTA");
        CV::text(120, 240, "T: liga e desliga a translacao da pista");
        CV::text(120, 260, "+: aumenta a pista");
        CV::text(120, 280, "-: diminui a pista");
        CV::text(120, 300, "Para rotacionar use os botoes do menu lateral");
        CV::text(120, 320, "MOVIMENTACAO DO CARRINHO");
        CV::text(120, 340, "A: acelera");
        CV::text(120, 360, "F: freia");
        CV::text(120, 380, "SETAS: mudam a direcao do carrinho");
        CV::text(120, 420, "O jogo acaba ao coletar 10 bolinhas");
    }
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
        case Utils::Delete:
            bezier->getControlPoints()->clearControlPoints();
            bezier->canApplyTransformations = false;
            bezier->translationMode = false;
            bezier->scale = 0;
            break;
        case Utils::KeyT:
            bezier->translationMode = (bezier->translationMode) ? false : true;
            break;
        case Utils::KeyPlus:
            bezier->rescaleCurve(1);
            break;
        case Utils::KeyMinus:
            bezier->rescaleCurve(-1);
            break;
        case Utils::KeyA:
            car->increaseSpeed();
            break;
        case Utils::KeyF:
            car->decreaseSpeed();
            break;
        case Utils::RightArrow:
            car->checkRotation(car->RightArrow);
            break;
        case Utils::LeftArrow:
            car->checkRotation(car->LeftArrow);
            break;
        case Utils::DownArrow:
            car->checkRotation(car->DownArrow);
            break;
        case Utils::UpArrow:
            car->checkRotation(car->UpArrow);
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
