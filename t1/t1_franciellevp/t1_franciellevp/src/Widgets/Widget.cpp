#include "Widget.h"

using namespace std;

Widget::Widget() {}

Widget::~Widget() {}

Widget::Widget(int x, int y, float w, float h, vector<float> bg)
{
    this->x = x;
    this->y = y;
    this->width = w;
    this->height  = h;
    this->bgColor = bg;
}

// REVER SE DEVE FICAR AQ OU N
// recebe as coordenadas do mouse para tratar clique ou detectar quando o mouse esta em cima do botao
bool Widget::Colidiu(int mx, int my)
{
    if(mx >= x && mx <= (x + width) && my >= y && my <= (y + height))
        return true;
    return false;
}
