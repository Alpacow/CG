#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "gl_canvas2d.h"

class Button
{
    float height, width, x, y;
    char label[100];
    vector<float> bgColor;

public:
    Button (float _x, float _y, float _w, float _h, char *_label, vector<float> _rgb)
    {
        height  = _h;
        width = _w;
        x = _x;
        y = _y;
        strcpy(label, _label);
        bgColor = _rgb;
    }

    void Render()
    {
        CV::color(bgColor[0], bgColor[1], bgColor[2]);
        CV::rectFill(x, y, x + width, y + height);
        CV::color(0, 0, 0);
        CV::text(x+5, y+height/2, label); //escreve o label do botao mais ou menos ao centro.
    }

    //recebe as coordenadas do mouse para tratar clique ou detectar quando o mouse esta em cima do botao
    bool Colidiu(int mx, int my)
    {
        if( mx >= x && mx <= (x + width) && my >= y && my <= (y + height) ) {
            return true;
        }
        return false;
    }

};

#endif
