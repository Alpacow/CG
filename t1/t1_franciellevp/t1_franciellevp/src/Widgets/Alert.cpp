#include "Alert.h"
#include "../Canvas/gl_canvas2d.h"
#include "../Controller/Controller.h"
#include "../Utils/Utils.h"
#include <iostream>

using namespace std;

Alert::Alert () : Widget()
{
    Create();
}

Alert::~Alert() {}

Alert::Alert(int x, int y, float w, float h, int hasInput, string text, vector<float> textColor, vector<float> bg)
    : Widget(), hasInput(hasInput), text(text), textColor(textColor)
{
    this->x = x;
    this->y = y;
    this->width = w;
    this->height  = h;
    this->bgColor = bg;
}

void Alert::Render()
{
    CV::color(bgColor[0], bgColor[1], bgColor[2]);
    CV::rectFill(x, y, x + width, y + height);
    CV::color(textColor[0], textColor[1], textColor[2]);
    CV::text(x+5, y+height/2, text.c_str());

    okButton = new Button(x+width+5, y+5+height/2, 50, 50, Utils::RGBtoFloat(28, 28, 28), "OK", Utils::RGBtoFloat(28, 28, 28));
    if (hasInput) {
        // TODO: desenhar input, permitindo entrada de dados
    }
}

void Alert::RenderWidgets()
{
    Render();
}

void Alert::CheckState(int state, int x, int y)
{
    if( state == 0 ) {
        if(Colidiu(x, y)) {
            cout << "Clicou no ALERT " << endl;
        }
    }
}

void Alert::Create()
{
    vector<float> txtColor = Utils::RGBtoFloat(65,105,225);
    vector<float> bg = Utils::RGBtoFloat(255, 250, 250);
    Controller* c = new Controller;
    alert = new Alert(c->getWidth()/2, c->getHeight()/2, 200, 100, 0, "Arquivo BMP nao tem largura multipla de 4", txtColor, bg);
}

