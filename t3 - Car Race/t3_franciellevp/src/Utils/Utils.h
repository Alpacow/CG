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
        LeftArrow = 200,
        UpArrow = 201,
        RightArrow = 202,
        DownArrow = 203,
    };

    static vector<float> RGBtoFloat(int r, int g, int b);
    static string getImagePath(const char* filename);
    static string getCurrentPath();
    static bool checkCircleCollision(int mx, int my, Vector2 p, int r);
    static bool checkRectCollision(int mx, int my, Vector2 topLeft, Vector2 bottomRight);
    static Vector2 rotatePoint(Vector2 p, Vector2 mid, float rad);
    static Vector2 intersecLines2d(Vector2 pLine1, Vector2 p2Line1, Vector2 pLine2, Vector2 p2Line2);
    static Vector2 getNearestPoint(Vector2 p1, Vector2 pc, Vector2 p2);
    static Vector2 getPointInQuadraticCurve (Vector2 t, Vector2 p1, Vector2 pc, Vector2 p2);
};

typedef struct // estrutura que guarda a cor de um pyxel
{
    unsigned char  b;
    unsigned char  g;
    unsigned char  r;
} __attribute__((__packed__))
Color;
#endif

