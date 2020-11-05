#include "Button.h"

using namespace std;

Button::Button ()
{
    createButtons ();
}

Button::Button (int _x, int _y, float _w, float _h, char *_label, vector<float> _rgb)
{
    height  = _h;
    width = _w;
    x = _x;
    y = _y;
    strcpy(label, _label);
    bgColor = _rgb;
}

void Button::Render()
{
    CV::color(bgColor[0], bgColor[1], bgColor[2]);
    CV::rectFill(x, y, x + width, y + height);
    CV::color(0, 0, 0);
    CV::text(x+5, y+height/2, label); //escreve o label do botao mais ou menos ao centro.
}

//recebe as coordenadas do mouse para tratar clique ou detectar quando o mouse esta em cima do botao
bool Button::Colidiu(int mx, int my)
{
    if(mx >= x && mx <= (x + width) && my >= y && my <= (y + height)) {
        return true;
    }
    return false;
}
void Button::renderButtons ()
{
    for(vector<Button>::size_type i = 0; i != bts.size(); i++) {
        bts[i].Render();
    }
}

void Button::checkButtonState (int state, int _x, int _y)
{
    for(vector<Button>::size_type i = 0; i != bts.size(); i++) {
        if( state == 0 ) {
            if(bts[i].Colidiu(_x, _y)) {
                cout << "Clicou no botao" << i << endl;
            }
        }
    }
}

void Button::createButtons ()
{
    vector<float> rgb = Utils::RGBtoFloat(72,61,139);
    bts.push_back(Button(10, 20, 140, 50, (char*)"b1", rgb));
    rgb = Utils::RGBtoFloat(106,90,205);
    bts.push_back(Button(180, 20, 140, 50, (char*)"b2", rgb));
}

