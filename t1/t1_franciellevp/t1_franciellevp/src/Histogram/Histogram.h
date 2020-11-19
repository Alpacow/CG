#ifndef __HISTOGRAM_H__
#define __HISTOGRAM_H__

#include "../Bitmap/Bmp.h"
#include <string>

using namespace std;

class Histogram
{
private:
    Bmp* img;
    int x, xWidth, y, yHeight;
    int offsetX, offsetY;
    int countR[256];
    int countG[256];
    int countB[256];
    int* maxR;
    int* maxG;
    int* maxB;
    int maxColor;
    vector<float> rgb;

    void drawMaxLine(int* maxRGB, int* countRGB);
    int  textOffset(int x);
    void drawXaxis();
    void drawYaxis();
    void drawHistogramRects(int* counter, int i);

public:
    Histogram();
    ~Histogram();
    Histogram(Bmp*img, int x, int y, int xWidth, int yHeight);
    void drawHistogramGraph();
    void countColors();
};

#endif

