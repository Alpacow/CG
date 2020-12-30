#include "Slider.h"

#include "../Canvas/gl_canvas2d.h"
#include "../Utils/Utils.h"
#include <iostream>

#define RAD 8
#define MIN_SPEEDWAY 30
#define MAX_SPEEDWAY 80

using namespace std;

Slider::Slider () : Widget()
{
    create();
    this->canDrag = false;
}

Slider::~Slider() {}

Slider::Slider(int x, int y, float w, float h, vector<float> bg, const string label, int minRange, int maxRange)
    : Widget(), label(label), minRange(minRange), maxRange(maxRange)
{
    this->p.x = x;
    this->p.y = y;
    this->width = w;
    this->height  = h;
    this->bgColor = bg;
    this->circle = Vector2{p.x, p.y};
    this->canDrag = false;
    this->currentValue = this->minRange;
}

float Slider::getCurrentValue()
{
    return currentValue;
}

void Slider::render()
{
    CV::color(bgColor[0], bgColor[1], bgColor[2]);
    CV::rectFill(p.x, p.y, p.x + width, p.y + height);
    CV::color(bgColor[0], bgColor[1], bgColor[2]);
    CV::circleFill(circle.x, circle.y, RAD, 100);
    CV::text(p.x, p.y - 15, label.c_str());
}

void Slider::renderWidgets()

{
    for (vector<Slider>::size_type i = 0; i != sliders.size(); i++)
        sliders[i]->render();
}

void Slider::checkState(int button, int state, int x, int y)
{
    for (vector<Slider>::size_type i = 0; i != sliders.size(); i++) {
        if (button == 0) {
            if (state == 0) {
                if (Utils::checkCircleCollision(x, y, sliders[i]->circle, RAD))
                    sliders[i]->canDrag = true;
            } else if (state == 1) // soltou o mouse
                sliders[i]->canDrag = false;
        } else if (button == -2) {
            if (sliders[i]->canDrag) {
                Slider* s = sliders[i];
                if (x >= s->p.x && x <= s->p.x + s->width) {
                    sliders[i]->circle.x = x;
                    float percentPx = (s->circle.x - s->p.x) * 100 / s->width;
                    float valueSlided = percentPx * (s->maxRange - s->minRange) / 100;
                    s->currentValue = s->minRange + valueSlided;
                }
            }
        }
    }
}

void Slider::create()
{
    vector<float> bg = Utils::RGBtoFloat(28,28,28);
    sliders.push_back(new Slider(1030, 400, 120, 2, bg, "Largura da pista", MIN_SPEEDWAY, MAX_SPEEDWAY));
}

void Slider::keyboardCheck(int key) {}

