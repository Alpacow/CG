//**************************************************************
//
// classe que controla o desenho e funcionalidade do histograma
// de ocorrencia de cada canal da imagem BMP
//
//**************************************************************
#ifndef __HISTOGRAM_H__
#define __HISTOGRAM_H__

#include "../Bitmap/Bmp.h"
#include <string>

using namespace std;

class Histogram
{
private:
    Bmp* img;
    int x, y;                    // posicao inicial
    int xWidth, yHeight;        // largura do eixo X e altura do eixo Y
    int offsetX, offsetY;      // espaco necessario entre o valor de cada cor
    int countR[256];          // contador dos tons de vermelho
    int countG[256];         // contador dos tons de verde
    int countB[256];        // contador dos tons de azul
    int* maxR;             // valor do tom vermelho com mais ocorrencia
    int* maxG;            // valor do tom verde com mais ocorrencia
    int* maxB;           // valor do tom azul com mais ocorrencia
    int maxColor;       // valor mais alto entre todos os tons
    vector<float> rgb; // controla as cores RGB

    void drawMaxLine(int* maxRGB, int* countRGB);
    int  textOffsetX(int x);
    void drawXaxis();
    void drawYaxis();
    void drawHistogramRects(int* counter, int i);

public:
    Histogram();
    ~Histogram();
    Histogram(Bmp* img, int x, int y, int xWidth, int yHeight);
    void drawHistogramGraph();
    void countColors();
};

#endif

