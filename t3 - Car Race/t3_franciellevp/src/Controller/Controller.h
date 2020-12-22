//*******************************************************************************
//
// classe para fazer o controle e comunicacao necessaria entre as demais classes
//
//*******************************************************************************
#ifndef ___CONTROLLER__H___
#define ___CONTROLLER__H___

#include "../Bezier/Bezier.h"
#include "../Car/Car.h"
#include "../Widgets/Alert.h"
#include "../Widgets/Input.h"
#include "../Utils/Fps.h"
#include <vector>

using namespace std;

class Controller
{
private:
    vector<float> rgb;           // controla cores passadas em RGB
    int opcao = 50;             // variavel para selecao do que sera exibido na canvas.
    int mx, my;                //variaveis do mouse para poder exibir dentro da render().
    float fps;
    Frames* frames;

public:
    Car* car;                // imagem a ser manipulada
    Bezier* bezier;                // imagem a ser manipulada
    Alert* alerts;
    int screenWidth, screenHeight;
    vector<Widget*> wds;    // controla todos os widgets do programa
    Controller();
    ~Controller();
    void InitCanvas();
    void Render();
    void Keyboard(int key);
    void Mouse(int button, int x, int y, int state);
};

#endif
