//*******************************************************************************
//
// classe para fazer o controle e comunicacao necessaria entre as demais classes
//
//*******************************************************************************
#ifndef ___CONTROLLER__H___
#define ___CONTROLLER__H___

#include "../Histogram/Histogram.h"
#include <vector>

using namespace std;

class Controller
{
private:
    vector<float> rgb;           // controla cores passadas em RGB
    int opcao = 50;             // variavel para selecao do que sera exibido na canvas.
    int mx, my;                //variaveis do mouse para poder exibir dentro da render().

public:
    Bmp* img;                // imagem a ser manipulada
    Histogram* hist;
    int screenWidth, screenHeight;
    Alert* alerts;
    vector<Widget*> wds;    // controla todos os widgets do programa
    Controller();
    ~Controller();
    void InitCanvas();
    void Render();
    void Keyboard(int key);
    void Mouse(int x, int y, int state);
};

#endif
