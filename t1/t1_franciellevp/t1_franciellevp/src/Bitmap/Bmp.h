//************************************************************
//
// classe para fazer o carregamento de arquivos no formato BMP
//
//************************************************************
#ifndef ___BMP__H___
#define ___BMP__H___

#include "../Utils/Utils.h"
#include "../Widgets/Alert.h"
#include <string>

#define HEADER_SIZE      14 //sizeof(HEADER) vai dar 16 devido ao alinhamento de bytes
#define INFOHEADER_SIZE  40 //sizeof(INFOHEADER) da 40 e esta correto.
#define uchar unsigned char

using namespace std;

class BmpHeader
{
public:
    short type;                 /* Magic identifier            */
    int size;                       /* File size in bytes          */
    short reserved1, reserved2;
    int offset;                     /* Offset to image data em bytes*/
};


class BmpInfoHeader
{
public:
    int size;               /* Header size in bytes      */
    int width, height;                /* Width and height of image */
    short planes;       /* Number of colour planes   */
    short bits;         /* Bits per pixel            */
    int compression;        /* Compression type          */
    int imagesize;          /* Image size in bytes       */
    int xresolution,yresolution;     /* Pixels per meter          */
    int ncolours;           /* Number of colours         */
    int impcolours;         /* Important colours         */
};


class Bmp
{
private:
    int width, height, imagesize, bytesPerLine, bits;
    Color** dt;
    BmpHeader header;
    BmpInfoHeader info;
    Alert** alert;

    void load(const char* fileName);
    void readHeader(FILE* fp);
    void readInfoHeader(FILE* fp);
    void rotateRight (Color** temp);
    void rotateLeft (Color** temp);

public:
    vector<int> channel; // controla visualizacao de cada componente

    Bmp(string fileName, Alert** alerts);
    Color** getImage();
    Color** newBitmap(int h, int w);
    int     getWidth(void);
    int     getHeight(void);
    void    deleteBitmap(Color** dt, int h, int w);
    void    renderBmp(int px, int py);
    void    mirrorV();
    void    mirrorH();
    void    rotate90(int clockwise);
    void    luminance();
    void    sepiaEffect();
    void    blurEffect();
    void    resizeBilinear(int newW, int newH);
    void    scale();
};

#endif
