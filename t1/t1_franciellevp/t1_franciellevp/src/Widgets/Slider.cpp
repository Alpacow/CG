#include "Slider.h"

#include "../Canvas/gl_canvas2d.h"
#include "../Utils/Utils.h"
#include <iostream>

using namespace std;

Slider::Slider (Bmp** img) : Widget()
{
    //this->imgController = img;
    Create();
}

Slider::~Slider() {}

Slider::Slider(int x, int y, float w, float h, vector<float> bg, int minRange, int maxRange, float inc = 1)
    : Widget(), minRange(minRange), maxRange(maxRange), inc(inc)
{
    this->x = x;
    this->y = y;
    this->width = w;
    this->height  = h;
    this->bgColor = bg;
}

void Slider::Render()
{
    vector<float> rgb = Utils::RGBtoFloat(255,250,250);
    CV::color(rgb[0], rgb[1], rgb[2]);
    CV::rectFill(x, y, x + width, y + height);
    CV::color(bgColor[0], bgColor[1], bgColor[2]);
    CV::circleFill(x + width, y, 8, 100);
}

void Slider::RenderWidgets()
{
    for(vector<Slider>::size_type i = 0; i != sliders.size(); i++)
        sliders[i]->Render();
}

void Slider::CheckState(int state, int x, int y)
{
    if( state == 0 ) {
        if(sliders[0]->Colidiu(x, y))
            cout << "mexendo no slider" << endl;
    }
}

void Slider::Create()
{
    vector<float> bg = Utils::RGBtoFloat(255,250,250);
    sliders.push_back(new Slider(820, 110, 200, 2, bg, 10, 100, 1));
}

