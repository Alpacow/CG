//************************************************************
//
// classe para fazer o carregamento de arquivos no formato BMP
//
//************************************************************

#include "Bmp.h"
#include "../Canvas/gl_canvas2d.h"

#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>

using namespace std;

Bmp::Bmp(string fileName, Alert** alerts)
{
    width = height = 0;
    channel = {1,1,1};
    alert = alerts;
    if(!fileName.empty() && fileName.size() > 0)
        load(fileName.c_str());
    else
        //(*alert)->alert = new Alert(350, 300, 500, 200, "Nome do arquivo BMP inválido", Utils::ERRO, TRUE);
        cout << "Error: Invalid BMP filename" << endl;
}

Color** Bmp::getImage()
{
    return dt;
}

int Bmp::getWidth(void)
{
    return width;
}

int Bmp::getHeight(void)
{
    return height;
}

Color** Bmp::newBitmap(int h, int w)
{
    Color** newdt = new Color*[h];
    for(int i = 0; i < h; i++)
        newdt[i] = new Color[w];
    return newdt;
}

void Bmp::deleteBitmap(Color** dt, int h, int w)
{
    for(int i = 0; i < h; i++)
        delete dt[i];
    delete dt;
}

void Bmp::renderBmp(int px, int py)
{
    for(int i = 0; i < height; i++)
        for(int j = 0; j < width; j++) {
            vector<float> rgb = Utils::RGBtoFloat(dt[i][j].r , dt[i][j].g , dt[i][j].b);
            // cores sao multiplicadas por channel para visualizacao dos canais
            CV::color(rgb[0] * channel[0], rgb[1] * channel[1], rgb[2] * channel[2]);
            CV::point(px + j, py + height - i);
        }
}

void Bmp::mirrorH()
{
    for (int i = 0; i < height; i++) {
        int y = width - 1;
        for (int j = 0; j < width/2; j++) {
            swap(dt[i][j], dt[i][y]);
            y--;
        }
    }
}

void Bmp::mirrorV()
{
    for (int i = 0; i < width; i++) {
        int y = height - 1;
        for (int j = 0; j < height/2; j++) {
            swap(dt[j][i], dt[y][i]);
            y--;
        }
    }
}

void Bmp::rotate90 (int clockwise)
{
    Color** temp = newBitmap(width, height);
    if (clockwise) rotateRight(temp);
    else rotateLeft(temp);
    deleteBitmap(dt, height, width);
    swap(height, width);
    dt = newBitmap(height, width);
    dt = temp;
}

void Bmp::rotateRight (Color** temp)
{
    for (int i = 0; i < height; i++) {
        int y = 0;
        for (int j = width-1; j >= 0; j--) {
            temp[y][i] = dt[i][j];
            y++;
        }
    }
}

void Bmp::rotateLeft(Color** temp)
{
    int y = height - 1;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            temp[j][y] = dt[i][j];
        }
        y--;
    }
}

void Bmp::luminance ()
{
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            float luminance = .299 * dt[i][j].r + .587 * dt[i][j].g + .114 * dt[i][j].b;
            dt[i][j].r = luminance;
            dt[i][j].g = luminance;
            dt[i][j].b = luminance;
        }
    }
}

void Bmp::sepiaEffect ()
{
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int r = dt[i][j].r;
            int g = dt[i][j].g;
            int b = dt[i][j].b;
            float gry = (r + g + b) / 3;
            b = gry;
            g = b;
            r = g;
            r += 20 * 2; // 20 é a profundidade
            g += 20;
            if (r > 255) r = 255;
            if (g > 255) g = 255;
            dt[i][j].r = r;
            dt[i][j].g = g;
            dt[i][j].b = b;
        }
    }
}

/* utiliza o método "blur box", para cada pixel, faz uma média(AVG) de todos seus vizinhos, incluindo a si proprio */
void Bmp::blurEffect()
{
    int sumBlue, sumGreen, sumRed;
    float countColor;
    Color temp[height][width];

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            sumBlue = sumGreen = sumRed = 0;
            countColor = 0.00;
            for (int k = -1; k < 2; k++) {
                if (j + k < 0 || j + k > height - 1) continue; // se  vizinho sai da imagem
                for (int h = -1; h < 2; h++) {
                    if (i + h < 0 || i + h > width - 1) continue;// se vizinho sai da imagem
                    sumBlue += dt[j+k][i+h].r;
                    sumGreen += dt[j+k][i+h].g;
                    sumRed += dt[j+k][i+h].b;
                    countColor++;
                }
            }
            temp[j][i].r = round(sumBlue / countColor);
            temp[j][i].g = round(sumGreen / countColor);
            temp[j][i].b = round(sumRed / countColor);
        }
    }
    for (int i = 0; i < width; i++)
        for (int j = 0; j < height; j++) {
            dt[j][i].r = temp[j][i].r;
            dt[j][i].g = temp[j][i].g;
            dt[j][i].b = temp[j][i].b;
        }
}

void Bmp::scale()
{
    int h = height/2;
    int w = width/2;
    Color** temp = newBitmap(h, w);

    int ix = 0;
    for (int i = 0; ix < h; i += 2) {
        int jx = 0;
        for (int j = 0; jx < w; j += 2) {
             temp[ix][jx] = dt[i][j];
             jx++;
        }
        ix++;
    }
    deleteBitmap(dt, height, width);
    dt = newBitmap(h, w);
    dt = temp;
    height = h;
    width = w;
}

/**
 */
void Bmp::resizeBilinear(int newW, int newH)
{
    Color** temp = newBitmap(width, height);
    Color a, b, c, d;
    int x, y;
    float xRatio = ((float)(width-1))/newW;
    float yRatio = ((float)(height-1))/newH;
    float xDiff, yDiff, blue, red, green;
    for (int i = 0; i < newH; i++) {
        for (int j = 0; j < newW; j++) {
            x = (int)(xRatio * j);
            y = (int)(yRatio * i);
            cout << x << " " << y << endl;
            xDiff = (xRatio * j) - x;
            yDiff = (yRatio * i) - y;
            a = temp[y][j];
            b = temp[y+1][j];
            c = temp[y][j+1];
            d = temp[y+1][j+1];

            // blue element
            // Yb = Ab(1-w)(1-h) + Bb(w)(1-h) + Cb(h)(1-w) + Db(wh)
            blue = a.b*(1-xDiff)*(1-yDiff) + b.b*(xDiff)*(1-yDiff) +
                   c.b*(yDiff)*(1-xDiff)   + d.b*(xDiff*yDiff);
            // green element
            // Yg = Ag(1-w)(1-h) + Bg(w)(1-h) + Cg(h)(1-w) + Dg(wh)
            green = a.g*(1-xDiff)*(1-yDiff) + b.g*(xDiff)*(1-yDiff) +
                    c.g*(yDiff)*(1-xDiff)   + d.g*(xDiff*yDiff);
            // red element
            // Yr = Ar(1-w)(1-h) + Br(w)(1-h) + Cr(h)(1-w) + Dr(wh)
            red = a.r*(1-xDiff)*(1-yDiff) + b.r*(xDiff)*(1-yDiff) +
                  c.r*(yDiff)*(1-xDiff)   + d.r*(xDiff*yDiff);
            cout << "CORES " << red << " " << green << " " << blue << endl;

            temp[i][j].b = blue;
            temp[i][j].g = green;
            temp[i][j].r = red;
        }
    }
    deleteBitmap(dt, height, width);
    dt = newBitmap(newH, newW);
    dt = temp;
    height = newH;
    width = newW;
}

//le o HEADER componente a componente devido ao problema de alinhamento de bytes. Usando
//o comando fread(header, sizeof(HEADER),1,fp) sao lidos 16 bytes ao inves de 14
void Bmp::readHeader(FILE* fp)
{
    fread(&header.type,      sizeof(unsigned short int), 1, fp);
    fread(&header.size,      sizeof(unsigned int),       1, fp);
    fread(&header.reserved1, sizeof(unsigned short int), 1, fp);
    fread(&header.reserved2, sizeof(unsigned short int), 1, fp);
    fread(&header.offset,    sizeof(unsigned int),       1, fp);
}

//le o INFOHEADER componente a componente devido ao problema de alinhamento de bytes
void Bmp::readInfoHeader(FILE* fp)
{
    fread(&info.size,        sizeof(unsigned int),       1, fp);
    fread(&info.width,       sizeof(int),                1, fp);
    fread(&info.height,      sizeof(int),                1, fp);
    fread(&info.planes,      sizeof(unsigned short int), 1, fp);
    fread(&info.bits,        sizeof(unsigned short int), 1, fp);
    fread(&info.compression, sizeof(unsigned int),       1, fp);
    fread(&info.imagesize,   sizeof(unsigned int),       1, fp);
    fread(&info.xresolution, sizeof(int),                1, fp);
    fread(&info.yresolution, sizeof(int),                1, fp);
    fread(&info.ncolours,    sizeof(unsigned int),       1, fp);
    fread(&info.impcolours,  sizeof(unsigned int),       1, fp);
}

void Bmp::load(const char* fileName)
{
    FILE *fp = fopen(fileName, "rb");
    if(fp == NULL) {
        (*alert)->alerts.push_back(new Alert(350, 300, 500, 200, "Erro ao abrir arquivo", Utils::ERRO, TRUE));
    }
    readHeader(fp);
    readInfoHeader(fp);
    width  = info.width;
    height = info.height;
    bits   = info.bits;
    bytesPerLine =(3 * (width + 1) / 4) * 4;
    imagesize    = bytesPerLine*height;
    int delta    = bytesPerLine - (3 * width);
    int padding  = (4 - (width * sizeof(Color)) % 4) % 4;

    cout << "Imagem: " << width << "x" << height << " - Bits: " << bits << endl;
    cout << "bytesPerLine: " << bytesPerLine << endl;
    cout << "bytesPerLine: " << width * 3 << endl;
    cout << "delta: " << delta << endl;
    cout << "imagesize: " << imagesize << " " << info.imagesize << endl;

    if( header.type != 19778 )
        (*alert)->alerts.push_back(new Alert(350, 300, 500, 200, "Arquivo BMP invalido ", Utils::ERRO, TRUE));
    if( width*height*3 != imagesize )
        (*alert)->alerts.push_back(new Alert(350, 300, 500, 200, "Arquivo BMP nao tem largura multipla de 4", Utils::WARNING, TRUE));
    if( info.compression != 0 )
        (*alert)->alerts.push_back(new Alert(350, 300, 500, 200, "Formato BMP comprimido nao suportado", Utils::ERRO, TRUE));
    if( bits != 24 )
        (*alert)->alerts.push_back(new Alert(350, 300, 500, 200, "Formato BMP nao suporta esse numero de bits", Utils::ERRO, TRUE));
    if( info.planes != 1 )
        (*alert)->alerts.push_back(new Alert(350, 300, 500, 200, "Numero de Planes nao suportado: ", Utils::ERRO, TRUE));
    dt = newBitmap(height, width);

    Color(*image)[width] = (Color(*)[width])calloc(height, width * sizeof(Color));
    for (int i = 0; i < height; i++) {
        fread(image[i], sizeof(Color), width, fp); // le todas a linha
        fseek(fp, padding, SEEK_CUR); // pula bytes extras, se houver
    }
    for(int i = 0; i < height; i++) // preenche o dt com as cores RGB da imagem
        for(int j = 0; j < width; j++)
            dt[i][j] = image[i][j];
    fclose(fp);
}
