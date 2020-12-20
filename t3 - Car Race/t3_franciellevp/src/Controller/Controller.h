//*******************************************************************************
//
// classe para fazer o controle e comunicacao necessaria entre as demais classes
//
//*******************************************************************************
#ifndef ___CONTROLLER__H___
#define ___CONTROLLER__H___

#include "../Bezier/Bezier.h"
#include "../Bitmap/Bmp.h"
#include "../Widgets/Alert.h"
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
    Bezier* bezier;                // imagem a ser manipulada
    int screenWidth, screenHeight;
    Alert* alerts;
    vector<Widget*> wds;    // controla todos os widgets do programa
    Controller();
    ~Controller();
    void InitCanvas();
    void Render();
    void Keyboard(int key);
    void Mouse(int button, int x, int y, int state);

    void rotateRect(float degrees);
    Vector2 rotatePoint(Vector2 p, Vector2 mid, float rad);
};

#endif
