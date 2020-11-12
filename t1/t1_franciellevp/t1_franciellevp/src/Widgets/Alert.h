#ifndef __ALERT_H__
#define __ALERT_H__

#include "Widget.h"
#include <string>

using namespace std;

class Alert : public Widget
{
public:
    string txt;
    vector<float> txtColor;
    vector<Alert*> alerts;
    int xb;
    int yb;
    int wb;
    int hb;

    Alert();
    ~Alert();
    Alert(int x, int y, float w, float h, const string txt, int type);
    bool Colidiu(int mx, int my);
    void Render() override;
    void CheckState(int state, int x, int y) override;
    void Create () override;
    void RenderWidgets() override;
};

#endif

