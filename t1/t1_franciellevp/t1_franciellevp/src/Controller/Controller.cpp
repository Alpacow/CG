//****************************************************************
//
// classe para desenhar e controlar o layout inicial do programa
//
//****************************************************************
#include "../Canvas/gl_canvas2d.h"
#include "Controller.h"
#include "../Widgets/Button.h"
#include "../Widgets/Checkbox.h"
#include <iostream>

using namespace std;

Controller::Controller()
{
    alerts = new Alert();
    wds.push_back(alerts);
    wds.push_back(new Button(&img));
    wds.push_back(new Checkbox(&img));
}

Controller::~Controller() {}

void Controller::Render()
{
    rgb = Utils::RGBtoFloat(28,28,28);
    CV::color(rgb[0], rgb[1], rgb[2]);
    CV::rectFill(0, 0, screenWidth, 30);

    rgb = Utils::RGBtoFloat(255,250,250);
    CV::color(rgb[0], rgb[1], rgb[2]);
    CV::text(screenWidth/2 - 80, 20, "T1 - Francielle Vasconcellos Pereira");
    CV::text(780, 50, "Girar 90 graus:");
    CV::text(660, 120, "Canais:");
    img->renderBmp(10, 40);
    hist->drawHistogramGraph();
    for(vector<Widget>::size_type i = 0; i != wds.size(); i++)
        wds[i]->RenderWidgets();
}

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

void Controller::Mouse(int x, int y, int state)
{
    mx = x; //guarda as coordenadas do mouse para exibir dentro da render()
    my = y;
    //cout << mx << " - " << my << endl;
    for(vector<Widget>::size_type i = 0; i != wds.size(); i++)
        wds[i]->CheckState(state, x, y);
}

void Controller::InitCanvas() {
    CV::init(&screenWidth, &screenHeight, "T1 - Visualizador de Imagens");
    img = new Bmp(Utils::getImagePath("normal_1.bmp"), &alerts);
    hist = new Histogram(img, 520, 550, 600, 300);
    rgb = Utils::RGBtoFloat(54,54,54);
    CV::clear(rgb[0], rgb[1], rgb[2]);
    CV::run();
}
