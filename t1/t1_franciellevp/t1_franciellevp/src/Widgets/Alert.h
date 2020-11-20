//******************************************************************************
//
// classe derivada de Widget que controla as janelas de mensagem da aplicacao
//
//******************************************************************************
#ifndef __ALERT_H__
#define __ALERT_H__

#include "Widget.h"
#include <string>

using namespace std;

class Alert : public Widget
{
public:
    string txt;
    int type; // WARNING, ERROR, INFO
    int isActive;
     vector<Alert*> alerts;
    int xb;
    int yb;
    int wb = 50;
    int hb = 30;

    Alert();
    ~Alert();
    Alert(int x, int y, float w, float h, const string txt, int type, int isActive);
    bool Colidiu(int mx, int my);
    void Render() override;
    void CheckState(int state, int x, int y) override;
    void Create () override;
    void RenderWidgets() override;
};

#endif

