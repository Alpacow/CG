//*************************************************************
//
// classe que contem funcoes auxiliares utilizadas na aplicacao
//
//*************************************************************
#ifndef ___UTILS__H___
#define ___UTILS__H___

#include "Vector2.h"
#include <vector>
#include <string>
using namespace std;

class Utils
{
public:
    enum // enum para as janelas de alerta
    {
        WARNING,
        ERRO,
        INFO
    };

    enum // teclas utilizadas
    {
        Esc = 27,
        Delete = 127,
    };

    static vector<float> RGBtoFloat(int r, int g, int b);
    static string getImagePath(const char* filename);
    static string getCurrentPath();
    static bool checkCircleCollision(int mx, int my, Vector2 p, int r);
};

typedef struct // estrutura que guarda a cor de um pyxel
{
    unsigned char  b;
    unsigned char  g;
    unsigned char  r;
} __attribute__((__packed__))
Color;
#endif

