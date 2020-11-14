//************************************************************
//
// classe para fazer o carregamento de arquivos no formato BMP
//
//************************************************************
#ifndef ___BMP__H___
#define ___BMP__H___

#include "../Utils/Utils.h"
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
    int width = 1, height = 1, imagesize, bytesPerLine, bits;
    Color** dt;
    BmpHeader header;
    BmpInfoHeader info;
    void load(const char* fileName);
    void readHeader(FILE* fp);
    void readInfoHeader(FILE* fp);

public:
    enum imgDirections
    {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };
    int direction;
    Bmp(string fileName);
    Color** getImage();
    int    getWidth(void);
    int    getHeight(void);
    void   convertBGRtoRGB(void);
    void   renderBmp(int px, int py);
    void   mirrorV();
    void   mirrorH();
};

#endif
