#include "../gl_canvas2d.h"
#include "Utils.h"

#include <unistd.h>
#include <iostream>

using namespace std;

static float calcRtoF (float i)
{
    float maxRGB = 255;
    return (i / maxRGB);
}

/* Converte RGB para float
   @param r: inteiro equivalente a RED
   @param g: inteiro equivalente a GREEN
   @param b: inteiro equivalente a BLUE
*/
vector<float> Utils::RGBtoFloat (int r, int g, int b)
{
    vector<float> rgb;
    rgb.push_back(calcRtoF(r));
    rgb.push_back(calcRtoF(g));
    rgb.push_back(calcRtoF(b));
    return rgb;
}
string Utils::getCurrentPath() {
    char buf[100];
    getcwd(buf, sizeof(buf));
    string path = buf;
    return path;
}

string Utils::getImagePath (const char* filename) {
    string path = getCurrentPath();
    const char* aux;
    #ifdef WIN32
        aux = "\\t1_franciellevp\\resources\\";
        path += aux;
    #else
        aux = "/t1_franciellevp/resources/";
        path += aux;
    #endif
    path += filename;
    return path;
}
