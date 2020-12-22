//****************************************************************
//
// classe derivada de Widget que controla os inputs da aplicacao
//
//****************************************************************
#ifndef __INPUT_H__
#define __INPUT_H__

#include "Widget.h"
#include <string>

using namespace std;

class Input : public Widget
{
private:
    bool canWrite;
    string buffer;

public:
    string label;
    vector<float> labelColor;
    vector<Input*> inputs;

    Input();
    ~Input();
    Input(int x, int y, float w, float h, vector<float> bg, const string label, vector<float> labelColor);
    bool getCanWrite();
    void render() override;
    void checkState(int state, int x, int y) override;
    void create () override;
    void renderWidgets() override;
    void keyboardCheck(int key) override;
};

#endif
