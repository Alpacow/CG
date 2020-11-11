#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "Widget.h"
#include <string>

using namespace std;

class Button : public Widget
{
public:
    string label;
    vector<float> labelColor;
    vector<Button*> bts;

    Button();
    ~Button();
    Button(int x, int y, float w, float h, vector<float> bg, const string label, vector<float> labelColor);
    void Render() override;
    void CheckState(int state, int x, int y) override;
    void Create () override;
    void RenderWidgets() override;
};

#endif

