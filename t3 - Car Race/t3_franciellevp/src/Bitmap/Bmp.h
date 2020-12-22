//*************************************************************************
//
// classe para fazer o carregamento e tratamento de arquivos no formato BMP
//
//**************************************************************************
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
    int size;                   /* File size in bytes          */
    short reserved1, reserved2;
    int offset;                 /* Offset to image data em bytes*/
};


class BmpInfoHeader
{
public:
    int size;                      /* Header size in bytes      */
    int width, height;             /* Width and height of image */
    short planes;                  /* Number of colour planes   */
    short bits;                    /* Bits per pixel            */
    int compression;               /* Compression type          */
    int imagesize;                 /* Image size in bytes       */
    int xresolution,yresolution;   /* Pixels per meter          */
    int ncolours;                  /* Number of colours         */
    int impcolours;                /* Important colours         */
};


class Bmp
{
private:
    int width, height, imagesize, bytesPerLine, bits;
    Color** dt;
    BmpHeader header;
    BmpInfoHeader info;
    Alert** alert;
    string path;
    Vector2 position;
    float rad;

    void load(const char* path);
    void readHeader(FILE* fp);
    void writeHeader(FILE* fp);
    void readInfoHeader(FILE* fp);
    void writeInfoHeader(FILE* fp);
    void rotateRight (float rad, Color** temp);
    void rotateLeft (float rad, Color** temp);

public:
    Bmp();
    Bmp(float x, float y, string _path, Alert** alerts);
    Color** getImage();
    Color** newBitmap(int h, int w);
    void    write();
    int     getWidth(void);
    int     getHeight(void);
    void    deleteBitmap(Color** dt, int h, int w);
    void    renderBmp();
    void    rotateImage(float rad, int clockwise);
};

#endif
