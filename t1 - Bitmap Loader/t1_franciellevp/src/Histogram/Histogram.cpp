//**************************************************************
//
// classe que controla o desenho e funcionalidade do histograma
// de ocorrencia de cada canal da imagem BMP
//
//**************************************************************
#include "Histogram.h"
#include "../Canvas/gl_canvas2d.h"
#include "../Utils/Utils.h"
#include <iostream>
#include <algorithm>

using namespace std;

Histogram::~Histogram() {}

/* Inicia os atributos necessarios
   @param img: ponteiro da instancia de Bmp para acessar os dados da imagem
   @param x: coordenada x onde inicia-se o desenho das linhas dos eixos do histograma
   @param y: coordenada y onde inicia-se o desenho das linhas dos eixos do histograma
   @param xWidth: largura do eixo X
   @param yHeight: altura do eixo Y
*/
Histogram::Histogram(Bmp*img, int x, int y, int xWidth, int yHeight)
{
    this->xWidth = xWidth;
    this->yHeight = yHeight;
    this->x = x;
    this->y = y;
    this->img = img;
    this->maxColor = 0;
    this->isLuminance = 1;
}

/* Desenha os eixos, dados e valores do histograma
*/
void Histogram::drawHistogramGraph()
{
    offsetX = xWidth/256;
    offsetY = yHeight/100;
    rgb = Utils::RGBtoFloat(255, 250, 250);
    CV::color(rgb[0], rgb[1], rgb[2]);
    CV::rectFill(x - 80, y + 50, x + xWidth + 50, y - yHeight - 50);
    drawXaxis();
    drawYaxis();
    countColors();
    for (int i = 0; i < 256; i++) {
        if (img->channel[0]) {
            CV::color(1, 0, 0);
            drawHistogramRects(countR, i);
        }
        if (img->channel[1]) {
            CV::color(0, 1, 0);
            drawHistogramRects(countG, i);
        }
        if (img->channel[2]) {
            CV::color(0, 0, 1);
            drawHistogramRects(countB, i);
        }
        if (isLuminance) {
            CV::color(0, 0, 0);
            drawHistogramRects(countL, i);
        }
    }
}

/* Retorna o espaco necessario na coordenada X para escrever o texto no eixoY do histograma
   @param x: o valor medio do eixoY do histograma (entre 0 e maxColor)
*/
int Histogram::textOffsetX(int x)
{
    if (x/100 < 1) return 25;
    else if (x/100 < 10) return 35;
    else if (x/ 100 < 100) return 45;
    else if (x/ 100 < 1000) return 55;
    else if (x/100 < 10000) return 65;
    return 0;
}

/* Conta os tons das cores de cada componente RGB da imagem
*/
void Histogram::countColors()
{
    maxR = maxG = maxB = maxL = 0;
    for(int i = 0; i < 256; i++) {
        countR[i] = 0;
        countG[i] = 0;
        countB[i] = 0;
        countL[i] = 0;
    }
    Color** dt = img->getImage();
    for(int i = 0; i < img->getHeight(); i++)
        for(int j = 0; j < img->getWidth(); j++) {
            Color c = dt[i][j];
            countR[(int)c.r]++;
            countG[(int)c.g]++;
            countB[(int)c.b]++;
            int lum = (c.r * .299) + (c.g * .587) + (c.b * .114);
            countL[lum]++;
        }
    maxR = max_element(begin(countR), end(countR));
    maxG = max_element(begin(countG), end(countG));
    maxB = max_element(begin(countB), end(countB));
    maxL = max_element(begin(countL), end(countL));
    maxColor = max(*maxR, *maxG);
    maxColor = max(maxColor, *maxB);
}

/* Desenha a linha, as divisoes da linha e os valores principais do eixoX do histograma
*/
void Histogram::drawXaxis()
{
    rgb = Utils::RGBtoFloat(28,28,28);
    CV::color(rgb[0], rgb[1], rgb[2]);
    CV::line(x, y, x + xWidth, y);
    CV::line(x + xWidth, y, x + xWidth - 20, y + 10); // flechinha
    CV::line(x + xWidth, y, x + xWidth - 20, y - 10);
    CV::text(x - 4, y + 16, "0"); // valores principais
    CV::text(x + (127 * offsetX) - 14, y + 16, "127");
    CV::text(x + (255 * offsetX) - 14, y + 16, "255");
    for(int i = 0; i < 256; i++) { // linhas do eixo X
        if (i == 0 || i == 127 || i == 255)
            CV::line(x + (i * offsetX), y - 6, x + (i * offsetX), y + 6);
        if (i % 10 == 0)
            CV::line(x + (i * offsetX), y - 2, x + (i * offsetX), y + 2);
    }
}

/* Desenha a linha, as divisoes da linha e os valores principais do eixoY do histograma
*/
void Histogram::drawYaxis()
{
    rgb = Utils::RGBtoFloat(28,28,28);
    CV::color(rgb[0], rgb[1], rgb[2]);
    CV::line(x, y - yHeight, x, y);
    CV::line(x, y - yHeight, x + 10, y - yHeight + 20); // flechinha
    CV::line(x, y - yHeight, x - 10, y - yHeight + 20);
    CV::text(x - 15, y - 4, "0"); // valores principais
    char buffer[7];
    int mid = maxColor/2;
    sprintf(buffer, "%d", mid);
    CV::text(x - textOffsetX(mid), y - (50 * offsetY) - 4, buffer);
    sprintf(buffer, "%d", maxColor);
    CV::text(x - textOffsetX(mid), y - (100 * offsetY) - 4, buffer);
    for(int i = 0; i < 100; i++) { // linhas do eixo Y
        if (i == 0 || i == 50 || i == 100)
            CV::line(x - 6, y - (i * offsetY), x + 6, y - (i * offsetY));
        if (i % 10 == 0)
            CV::line(x - 4, y - (i * offsetY), x + 4, y - (i * offsetY));
    }
}

/* Desenha os retangulos que representam os valores dos dados da imagem no histograma
   @param counter: array que contem a contagem de cada tom do componente RGB
   @param i: posicao do array que contem o valor a ser desenhada
*/
void Histogram::drawHistogramRects(int* counter, int i)
{
    if (maxColor != 0) { // maxColor = 0 quando ha problema ao carregar/abrir a imagem
        int value = (int)((counter[i] * 100) / maxColor); // regra de 3 p saber pos de y
        CV::rectFill(x + (i * offsetX), y, x + (i * offsetX) + offsetX, y - (value * offsetY));
    }
}

