#include "Checkbox.h"
#include "../Canvas/gl_canvas2d.h"
#include "../Utils/Utils.h"
#include <iostream>

#define FALSE 0
#define TRUE 1

using namespace std;

Checkbox::Checkbox () : Widget()
{
    Create();
}

Checkbox::~Checkbox() {}

Checkbox::Checkbox(int x, int y, float w, float h, int isChecked, string label, vector<float> labelColor)
    : Widget(), isChecked(isChecked), label(label), labelColor(labelColor)
{
    this->x = x;
    this->y = y;
    this->width = w;
    this->height  = h;
    this->bgColor = Utils::RGBtoFloat(255,250,250);
}

void Checkbox::Render()
{
    CV::color(bgColor[0], bgColor[1], bgColor[2]);
    CV::rectFill(x, y, x + width, y + height);
    vector<float> c = Utils::RGBtoFloat(65,105,225);
    CV::color(c[0], c[1], c[2]);
    CV::circle(x + width/2, y + height/2, width/3, 100);
    CV::color(labelColor[0], labelColor[1], labelColor[2]);
    CV::text(x+width+5, y+5+height/2, label.c_str());
    if (isChecked) {
        CV::color(c[0], c[1], c[2]);
        CV::circleFill(x + width/2, y + height/2, width/3, 100);
    }
}

void Checkbox::RenderWidgets()
{
    for(vector<Checkbox>::size_type i = 0; i != check.size(); i++)
        check[i]->Render();
}

void Checkbox::CheckState(int state, int x, int y)
{
    for(vector<Checkbox>::size_type i = 0; i != check.size(); i++) {
        if( state == 0 ) {
            if(check[i]->Colidiu(x, y)) {
                cout << "Clicou no checkbox " << i << endl;
                check[i]->isChecked = (check[i]->isChecked) ? FALSE : TRUE;
            }
        }
    }
}

void Checkbox::Create()
{
    vector<float> labelColor = Utils::RGBtoFloat(65,105,225);
    check.push_back(new Checkbox(400, 20, 20, 20, FALSE, "Check 1", labelColor));
    check.push_back(new Checkbox(400, 50, 20, 20, FALSE, "Check 2", labelColor));
}

