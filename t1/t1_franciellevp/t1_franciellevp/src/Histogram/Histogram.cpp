#include "Histogram.h"

#include "../Canvas/gl_canvas2d.h"
#include "../Utils/Utils.h"
#include <iostream>
#include <algorithm>

using namespace std;

Histogram::~Histogram() {}

Histogram::Histogram(Bmp*img, int x, int y, int xWidth, int yHeight)
{
    this->xWidth = xWidth;
    this->yHeight = yHeight;
    this->x = x;
    this->y = y;
    this->img = img;
    this->maxColor = 0;
}

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
        if (img->channel[3]) {
            CV::color(0, 0, 0);
            drawHistogramRects(countB, i); // reaproveita o vetor de Blue para a luminancia
        }
    }
}

int Histogram::textOffset(int x)
{
    if (x/100 < 1) return 25;
    else if (x/100 < 10) return 35;
    else if (x/ 100 < 100) return 45;
    else if (x/ 100 < 1000) return 55;
    else if (x/100 < 10000) return 65;
    return 0;
}

void Histogram::countColors()
{
    maxR = 0;
    maxG = 0;
    maxB = 0;
    for(int i = 0; i < 256; i++) {
        countR[i] = 0;
        countG[i] = 0;
        countB[i] = 0;
    }
    Color** dt = img->getImage();
    for(int i = 0; i < img->getHeight(); i++)
        for(int j = 0; j < img->getWidth(); j++) {
            Color c = dt[i][j];
            countR[(int)c.r]++;
            countG[(int)c.g]++;
            countB[(int)c.b]++;
        }
    maxR = max_element(begin(countR), end(countR));
    maxG = max_element(begin(countG), end(countG));
    maxB = max_element(begin(countB), end(countB));
    maxColor = max(*maxR, *maxG);
    maxColor = max(maxColor, *maxB);
}

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

void Histogram::drawYaxis()
{
    rgb = Utils::RGBtoFloat(28,28,28);
    CV::color(rgb[0], rgb[1], rgb[2]);
    CV::line(x, y - yHeight, x, y);
    CV::line(x, y - yHeight, x + 10, y - yHeight + 20); // flechinha
    CV::line(x, y - yHeight, x - 10, y - yHeight + 20);
    CV::text(x - 15, y - 4, "0"); // valores principais
    CV::text(x + (127 * offsetX) - 14, y + 16, "127");
    CV::text(x + (255 * offsetX) - 14, y + 16, "255");
    char buffer[7];
    int mid = maxColor/2;
    sprintf(buffer, "%d", mid);
    CV::text(x - textOffset(mid), y - (50 * offsetY) - 4, buffer);
    sprintf(buffer, "%d", maxColor);
    CV::text(x - textOffset(mid), y - (100 * offsetY) - 4, buffer);
    for(int i = 0; i < 100; i++) { // linhas do eixo Y
        if (i == 0 || i == 50 || i == 100)
            CV::line(x - 6, y - (i * offsetY), x + 6, y - (i * offsetY));
        if (i % 10 == 0)
            CV::line(x - 4, y - (i * offsetY), x + 4, y - (i * offsetY));
    }
}

void Histogram::drawHistogramRects(int* counter, int i)
{
    int pos = (int)((counter[i] * 100) / maxColor); // regra de 3 p saber pos de y
    CV::rectFill(x + (i * offsetX), y, x + (i * offsetX) + offsetX, y - (pos * offsetY));
}

