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
    static vector<float> RGBtoFloat(int r, int g, int b);
    static string getPath(const char* filename);
};
#endif

