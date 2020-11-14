//************************************************************
//
// classe para fazer o carregamento de arquivos no formato BMP
//
//************************************************************

#include "Bmp.h"
#include "../Canvas/gl_canvas2d.h"
#include "../Controller/Controller.h"

#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>

using namespace std;

Bmp::Bmp(string fileName)
{
    width = height = direction = 0;
    if(!fileName.empty() && fileName.size() > 0)
        load(fileName.c_str());
    else
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

void Bmp::renderBmp(int px, int py)
{
    int x = 0;
    for(int i = 0; i < height; i++) {
        for(int j = 0; j < width; j++) {
            vector<float> rgb = Utils::RGBtoFloat(dt[i][j].r , dt[i][j].g , dt[i][j].b);
            CV::color(rgb[0], rgb[1], rgb[2]);
            //CV::point(px + j, py + height - i); // img certa
            //CV::point(px + i, py + j); // virado p esquerda
            //CV::point(px + j, py + i); // cabeça p baixo
            CV::point(px + height - i, py + j); // virado p direita
            x += 3;
        }
    }
}

void Bmp::mirrorV()
{
    for (int i = 0; i < height; i++) {
        int y = width - 1;
        for (int j = 0; j < width/2; j++) {
            Color c = dt[i][j];
            dt[i][j] = dt[i][y];
            dt[i][y] = c;
            y--;
        }
    }
}

void Bmp::mirrorH()
{
    for (int i = 0; i < width; i++) {
        int y = height - 1;
        for (int j = 0; j < height/2; j++) {
            Color c = dt[j][i];
            dt[j][i] = dt[y][i];
            dt[y][i] = c;
            y--;
        }
    }
}

//le o HEADER componente a componente devido ao problema de alinhamento de bytes. Usando
//o comando fread(header, sizeof(HEADER),1,fp) sao lidos 16 bytes ao inves de 14
void Bmp::readHeader(FILE* fp) {
    fread(&header.type,      sizeof(unsigned short int), 1, fp);
    fread(&header.size,      sizeof(unsigned int),       1, fp);
    fread(&header.reserved1, sizeof(unsigned short int), 1, fp);
    fread(&header.reserved2, sizeof(unsigned short int), 1, fp);
    fread(&header.offset,    sizeof(unsigned int),       1, fp);
}

//le o INFOHEADER componente a componente devido ao problema de alinhamento de bytes
void Bmp::readInfoHeader(FILE* fp) {
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
        cout << "Erro ao abrir arquivo " << fileName << " para leitura" << endl;
        return;
    }
    cout << "\nCarregando arquivo " << fileName << endl;
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

    if( header.type != 19778 ) {
        cout << "Error: Arquivo BMP invalido" << endl;
        getchar();
        exit(0);
    }
    if( width*height*3 != imagesize ) {
        cout << width*height*3 << " " << imagesize << endl;
        cout << "Warning: Arquivo BMP nao tem largura multipla de 4" << endl;
        //getchar();
    }
    if( info.compression != 0 ) {
        cout << "Error: Formato BMP comprimido nao suportado" <<  endl;
        getchar();
        return;
    }
    if( bits != 24 ) {
        cout << "Error: Formato BMP com %d bits/pixel nao suportado" << endl;
        getchar();
        return;
    }
    if( info.planes != 1 ) {
        cout << "Error: Numero de Planes nao suportado: " << info.planes << endl;
        getchar();
        return;
    }
    dt = new Color*[height];
    for(int i = 0; i < height; i++)
        dt[i] = new Color[width];

    Color(*image)[width] = (Color(*)[width])calloc(height, width * sizeof(Color));
    for (int i = 0; i < height; i++) {
        fread(image[i], sizeof(Color), width, fp); // read all row
        fseek(fp, padding, SEEK_CUR); // skip padding
    }
    for(int i = 0; i < height; i++) // fill dt with RGB values of image
        for(int j = 0; j < width; j++)
            dt[i][j] = image[i][j];
    fclose(fp);
}
