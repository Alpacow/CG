//*************************************************************
//
// classe que contem funcoes auxiliares utilizadas na aplicacao
//
//*************************************************************
#include "../Canvas/gl_canvas2d.h"
#include "Utils.h"
#include <unistd.h>
#include <iostream>
#include <math.h>

using namespace std;

/* Metodo estatico auxiliar ao RGBtoFloat
   @param i: numero equivalente a RED, GREEN ou BLUE
   @return : ponto flutuante equivalente a componente RGB
*/
static float calcRtoF(float i)
{
    float maxRGB = 255;
    return (i / maxRGB);
}

/* Metodo estatico auxiliar ao FloatToRGB
   @param i: numero equivalente a RED, GREEN ou BLUE
   @return : ponto flutuante equivalente a componente RGB
*/
static int calcFtoR(float i)
{
    float maxRGB = 255;
    return (int)(i * maxRGB);
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

/* Converte float para RGB
   @param r: numero equivalente a RED
   @param g: numero equivalente a GREEN
   @param b: numero equivalente a BLUE
*/
vector<int> Utils::FloatToRGB (float r, float g, float b)
{
    vector<int> rgb;
    rgb.push_back(calcFtoR(r));
    rgb.push_back(calcFtoR(g));
    rgb.push_back(calcFtoR(b));
    return rgb;
}

/* Pega o caminho fisico em que os arquivos se encontram
   @return path: retorna todo o endereco fisico em uma string
*/
string Utils::getCurrentPath() {
    char buf[100];
    getcwd(buf, sizeof(buf));
    string path = buf;
    return path;
}

/* Pega o caminho fisico que a imagem desejada se encontra
   @param filename: nome da imagem bmp
   @return path: endereco fisico que a imagem se encontra
*/
string Utils::getImagePath (const char* filename) {
    string path = getCurrentPath();
    const char* aux;
    #ifdef WIN32
        aux = "\\t3_franciellevp\\resources\\";
        path += aux;
    #else
        aux = "/t3_franciellevp/resources/";
        path += aux;
    #endif
    path += filename;
    return path;
}

/* Recebe os pontos x e y do mouse e verificar se estão colidindo com os pontos no ponto P
    @param mx: ponto x do mouse
    @param my: ponto y do mouse
    @param p: vetor de ponto a verificar colisão
    @param r: raio do ponto p
    @return booleano de colisão
*/
bool Utils::checkCircleCollision(int mx, int my, Vector2 p, int r)
{
    return (mx >= (p.x - r) && mx <= (p.x + r) && my >= (p.y - r) && my <= (p.y + r));
}

/* Recebe os pontos x e y do mouse e verificar se estão colidindo com os pontos de topLeft e bottomRight no retangulo
    @param mx: ponto x do mouse
    @param my: ponto y do mouse
    @param topLeft: vetor de pontos do canto superior esquerdo do retangulo
    @param bottomRight: vetor de pontos do canto inferior direito do retangulo
    @return booleano de colisão
*/
bool Utils::checkRectCollision(int mx, int my, Vector2 topLeft, Vector2 bottomRight)
{
    return (mx >= topLeft.x && mx <= bottomRight.x && my >= topLeft.y && my <= bottomRight.y);
}

/* Recebe o vetor p e seu ponto central mid, e calcula os novos valores com base na rotação na variável rad
    @param p: vetor de pontos a rotacionar
    @param mid: vetor de pontos centrais
    @param rad: valor em radianos da rotação
    @return novo vetor de pontos rotacionais
*/
Vector2 Utils::rotatePoint(Vector2 p, Vector2 mid, float rad) {
    float a = p.x - mid.x;
    float b = p.y - mid.y;
    float xx = +a * cos(rad) - b * sin(rad) + mid.x;
    float yy = +a * sin(rad) + b * cos(rad) + mid.y;
    return Vector2{xx, yy};
}

/* Verifica a intersecção entre duas linhas
    @param pLine1: ponto 1 da linha 1
    @param p2Line1: ponto 2 da linha 1
    @param pLine2: ponto 1 da linha 2
    @param p2Line2: ponto 2 da linha 2
    @return retorna o ponto caso haja intersecção ou (0,0) em caso negativo
*/
Vector2 Utils::intersecLines2d(Vector2 pLine1, Vector2 p2Line1, Vector2 pLine2, Vector2 p2Line2)
{
    double det;
    Vector2 point;

    det = (p2Line2.x - pLine2.x) * (p2Line1.y - pLine1.y)  -  (p2Line2.y - pLine2.y) * (p2Line1.x - pLine1.x);
    if (det == 0.0)
        return Vector2 {0, 0};
    double s = ((p2Line2.x - pLine2.x) * (pLine2.y - pLine1.y) - (p2Line2.y - pLine2.y) * (pLine2.x - pLine1.x))/ det ;
    point.x = pLine1.x + (p2Line1.x - pLine1.x) * s; // eq parametrica da linha1
    point.y = pLine1.y + (p2Line1.y - pLine1.y) * s;
    return point;
}

/* Verifica a distancia entre 2 pontos
    @param p1: ponto 1
    @param p2: ponto 2
    @return retorna a distancia entre os 2 pontos
*/
float Utils::distanceBetweenPoints(Vector2 p1, Vector2 p2)
{
    return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
}
