#include "Button.h"

#include "../Canvas/gl_canvas2d.h"
#include "../Utils/Utils.h"
#include <iostream>

using namespace std;

Button::Button () : Widget()
{
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
    for(vector<Button>::size_type i = 0; i != bts.size(); i++) {
        if( state == 0 ) {
            if(bts[i]->Colidiu(x, y)) {
                cout << "Clicou no botao " << i << endl;
            }
        }
    }
}

void Button::Create()
{
    vector<float> bg = Utils::RGBtoFloat(72,61,139);
    vector<float> labelColor = Utils::RGBtoFloat(28, 28, 28);
    bts.push_back(new Button(10, 20, 140, 50, bg, "b1", labelColor));
    bg = Utils::RGBtoFloat(106,90,205);
    bts.push_back(new Button(180, 20, 140, 50, bg, "b2", labelColor));
}

