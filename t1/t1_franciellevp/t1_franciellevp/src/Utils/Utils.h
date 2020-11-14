//*************************************************************
//
// classe que contém funções auxiliares utilizadas na aplicação
//
//*************************************************************

#ifndef ___UTILS__H___
#define ___UTILS__H___

#include <vector>
#include <string>
using namespace std;

class Utils
{
public:
    enum
    {
        WARNING,
        ERRO,
        INFO
    };

    static vector<float> RGBtoFloat(int r, int g, int b);
    static string getImagePath(const char* filename);
    static string getCurrentPath();
};

typedef struct
{
    uint8_t  b;
    uint8_t  g;
    uint8_t  r;
} __attribute__((__packed__))
Color;
#endif

