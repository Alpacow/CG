#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <iostream>

#include "gl_canvas2d.h"
#include "Utils.h"

class Button
{
public:
    float height, width;
    char label[100];
    vector<float> bgColor;
    int x, y;
    vector<Button> bts;

    Button();
    Button (int _x, int _y, float _w, float _h, char *_label, vector<float> _rgb);
    void Render();
    //recebe as coordenadas do mouse para tratar clique ou detectar quando o mouse esta em cima do botao
    bool Colidiu(int mx, int my);
    void renderButtons ();
    void checkButtonState (int state, int _x, int _y);
    void createButtons ();
};

#endif

