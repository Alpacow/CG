#ifndef __ALERT_H__
#define __ALERT_H__

#include "Widget.h"
#include "Button.h"
#include <string>

using namespace std;

class Alert : public Widget
{
public:
    int hasInput; // se o alert vai possuir campo de entrada
    string text;
    vector<float> textColor;
    Button* okButton;
    Alert* alert;

    Alert();
    ~Alert();
    Alert(int x, int y, float w, float h, int hasInput, string text, vector<float> textColor, vector<float> bg);
    void Render() override;
    void CheckState(int state, int x, int y) override;
    void Create () override;
    void RenderWidgets() override;
};

#endif

