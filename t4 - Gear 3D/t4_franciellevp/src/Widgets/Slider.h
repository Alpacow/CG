#ifndef __SLIDER_H__
#define __SLIDER_H__

#include "Widget.h"
#include <string>

using namespace std;

class Slider : public Widget
{
private:
    Vector2 circle;
    bool canDrag;
    float currentValue;
    string label;

public:
    vector<Slider*> sliders;
    int minRange;
    int maxRange;

    Slider();
    ~Slider();
    Slider(int x, int y, float w, float h, vector<float> bg, const string label, int minRange, int maxRange);
    void render() override;
    void checkState(int button, int state, int x, int y) override;
    void create () override;
    void renderWidgets() override;
    void keyboardCheck(int key) override;
    float getCurrentValue();
};

#endif

