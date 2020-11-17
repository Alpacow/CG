#ifndef __SLIDER_H__
#define __SLIDER_H__

#include "Widget.h"
#include "../Bitmap/Bmp.h"
#include <string>

using namespace std;

class Slider : public Widget
{
public:
    vector<Slider*> sliders;
    int minRange;
    int maxRange;
    float inc;
    Bmp** imgController;

    Slider();
    Slider(Bmp** imgController);
    ~Slider();
    Slider(int x, int y, float w, float h, vector<float> bg, int minRange, int maxRange, float inc);
    void Render() override;
    void CheckState(int state, int x, int y) override;
    void Create () override;
    void RenderWidgets() override;
};

#endif

