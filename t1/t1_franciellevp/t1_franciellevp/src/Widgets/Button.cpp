#include "Button.h"

#include "../Canvas/gl_canvas2d.h"
#include "../Utils/Utils.h"
#include <iostream>

using namespace std;

Button::Button (Bmp** img) : Widget()
{
    this->imgController = img;
    Create();
}

Button::~Button() {}

Button::Button(int x, int y, float w, float h, vector<float> bg, const string label, vector<float> labelColor)
    : Widget(), label(label), labelColor(labelColor)
{
    this->x = x;
    this->y = y;
    this->width = w;
    this->height  = h;
    this->bgColor = bg;
}

void Button::Render()
{
    CV::color(bgColor[0], bgColor[1], bgColor[2]);
    CV::rectFill(x, y, x + width, y + height);
    CV::color(labelColor[0], labelColor[1], labelColor[2]);
    CV::text(x+5, y+height/2, label.c_str()); //escreve o label do botao mais ou menos ao centro.
}

void Button::RenderWidgets()
{
    for(vector<Button>::size_type i = 0; i != bts.size(); i++)
        bts[i]->Render();
}

void Button::CheckState(int state, int x, int y)
{
    if( state == 0 ) {
        if(bts[0]->Colidiu(x, y))
            (*imgController)->rotateRight();
        else if(bts[1]->Colidiu(x, y))
            (*imgController)->rotateLeft();
        else if(bts[2]->Colidiu(x, y))
            (*imgController)->sepiaEffect();
        else if(bts[3]->Colidiu(x, y))
            (*imgController)->luminance();
    }
}

void Button::Create()
{
    vector<float> bg = Utils::RGBtoFloat(255,250,250);
    vector<float> labelColor = Utils::RGBtoFloat(28, 28, 28);
    bts.push_back(new Button(880, 60, 100, 20, bg, "Horario", labelColor));
    bts.push_back(new Button(720, 60, 130, 20, bg, "Anti-horario", labelColor));

    bg = Utils::RGBtoFloat(210,180,140);
    bts.push_back(new Button(420, 100, 130, 20, bg, "Efeito Sepia", labelColor));
    bg = Utils::RGBtoFloat(169,169,169);
    bts.push_back(new Button(420, 130, 130, 20, bg, "Luminancia", labelColor));
}

