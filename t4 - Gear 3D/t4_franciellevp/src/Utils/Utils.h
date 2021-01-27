//*************************************************************
//
// classe que contem funcoes auxiliares utilizadas na aplicacao
//
//*************************************************************
#ifndef ___UTILS__H___
#define ___UTILS__H___

#include "Vector2.h"
#include "Vector3.h"
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
        INFO,
        SUCCESS
    };

    enum {
        X,
        Y,
        Z
    };

    enum // teclas utilizadas
    {
        Esc = 27,
        Delete = 127,
        KeyT = 116,
        KeyMinus = 45,
        KeyPlus = 61,
        KeyS = 115,
        KeyD = 100,
        LeftArrow = 200,
        UpArrow = 201,
        RightArrow = 202,
        DownArrow = 203,
        KeyZ = 122,
        KeyX = 120,
        KeyY = 121
    };

    static vector<float> RGBtoFloat(int r, int g, int b);
    static vector<int> FloatToRGB (float r, float g, float b);
    static string getImagePath(const char* filename);
    static string getCurrentPath();
    static bool checkCircleCollision(int mx, int my, Vector2 p, int r);
    static bool checkRectCollision(int mx, int my, Vector2 topLeft, Vector2 bottomRight);
    //static Vector2 rotatePoint(Vector2 p, Vector2 mid, float rad);
    static Vector2 intersecLines2d(Vector2 pLine1, Vector2 p2Line1, Vector2 pLine2, Vector2 p2Line2);
    static float distanceBetweenPoints(Vector2 p1, Vector2 p2);
    static Vector3 rotatePoint(Vector3 p,float rad, int axis);
    static Vector3 translate(Vector3 p, Vector3 o);
};

typedef struct // estrutura que guarda a cor de um pyxel
{
    unsigned char  b;
    unsigned char  g;
    unsigned char  r;
} __attribute__((__packed__))
Color;
#endif

