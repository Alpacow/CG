#ifndef __CHECKBOX_H__
#define __CHECKBOX_H__

#include "Widget.h"
#include "../Bitmap/Bmp.h"
#include <string>

using namespace std;

class Checkbox : public Widget
{
public:
    int isChecked;
    string label;
    vector<float> labelColor;
    vector<Checkbox*> check;
    Bmp** imgController;

    Checkbox();
    Checkbox(Bmp** img);
    ~Checkbox();
    Checkbox(int x, int y, float w, float h, int isChecked, string label, vector<float> labelColor, vector<float> bg);
    void render() override;
    void checkState(int state, int x, int y) override;
    void create () override;
    void renderWidgets() override;
};

#endif

