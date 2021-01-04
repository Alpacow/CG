//****************************************************************
//
// classe derivada de Widget que controla os botoes da aplicacao
//
//****************************************************************
#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "Widget.h"
#include "../Bezier/Bezier.h"
#include "../Car/Car.h"
#include <string>

using namespace std;

class Button : public Widget
{
public:
    string label;
    vector<float> labelColor;
    vector<Button*> bts;
    Bezier** bezierController;
    Car** carController;

    Button();
    Button(Bezier** b, Car** c);
    ~Button();
    Button(int x, int y, float w, float h, vector<float> bg, const string label, vector<float> labelColor);
    void render() override;
    void checkState(int button, int state, int x, int y) override;
    void create () override;
    void renderWidgets() override;
    void keyboardCheck(int key) override;
};

#endif

