//****************************************************************
//
// classe para desenhar e controlar o layout inicial do programa
//
//****************************************************************

#include "../Canvas/gl_canvas2d.h"
#include "../Utils/Utils.h"
#include "Controller.h"
#include <iostream>

using namespace std;

Controller::Controller()
{
    wds.push_back(new Button());
    wds.push_back(new Checkbox());
}

Controller::~Controller() {}

void Controller::Render()
{
    rgb = Utils::RGBtoFloat(255,250,250);
    CV::color(rgb[0], rgb[1], rgb[2]);
    CV::text(screenWidth/2 - 80, 20, "T1 - Francielle Vasconcellos Pereira");
    for(vector<Widget>::size_type i = 0; i != wds.size(); i++)
        wds[i]->RenderWidgets();
    if (opcao == 200)
        img->setFlip(0);
    if (opcao == 202)
        img->setFlip(1);
    img->renderBmp(250, 200);
}

void Controller::Keyboard(int key)
{
    cout << endl << "Tecla: " << key << endl;
    opcao = key;
    switch(key) {
        case 27:
            exit(0);
            break;
    }
}

void Controller::Mouse(int x, int y, int state)
{
    mx = x; //guarda as coordenadas do mouse para exibir dentro da render()
    my = y;
    for(vector<Widget>::size_type i = 0; i != wds.size(); i++)
        wds[i]->CheckState(state, x, y);
}

void Controller::InitCanvas() {
    CV::init(&screenWidth, &screenHeight, "T1 - Visualizador de Imagens");
    img = new Bmp(Utils::getImagePath("teste2.bmp"));
    rgb = Utils::RGBtoFloat(54,54,54);
    CV::clear(rgb[0], rgb[1], rgb[2]);
    CV::run();
}

int Controller::getHeight()
{
    return this->screenHeight;
}

int Controller::getWidth()
{
    return this->screenWidth;
}
