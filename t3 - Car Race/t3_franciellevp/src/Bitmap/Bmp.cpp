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
#include <math.h>

using namespace std;

Bmp::Bmp(){}

/* Inicializa atributos necessarios
   @param _fileName: nome da imagem bmp
   @param alerts: instancia da classe Alert para manipular janelas de mensagem
*/
Bmp::Bmp(float x, float y, string _path, Alert** alerts)
{
    position.x = x;
    position.y = y;
    width = height = 0;
    alert = alerts;
    path = _path;
    rad = 0;
    if(!path.empty() && path.size() > 0)
        load(path.c_str());
    else {
        (*alert)->alerts.push_back(new Alert(350, 300, 500, 200, "Nome do arquivo BMP invalido", Utils::ERRO, TRUE));
        return;
    }
}
/* Retorna a matriz que contem os dados da imagem bmp
   @return dt: matriz do tipo Color
*/
Color** Bmp::getImage()
{
    return dt;
}

/* Retorna a largura da imagem
   @return width: inteiro referente a largura do bmp
*/
int Bmp::getWidth(void)
{
    return width;
}

/* Retorna a altura da imagem
   @return height: inteiro referente a altura do bmp
*/
int Bmp::getHeight(void)
{
    return height;
}

/* Cria espaco em memoria para uma nova matriz de Color
   @param h: altura do bmp, ou numero de linhas da matriz
   @param w: largura do bmp, ou numero de colunas da matriz
   @return newdt: matriz do tipo Color
*/
Color** Bmp::newBitmap(int h, int w)
{
    Color** newdt = new Color*[h];
    for(int i = 0; i < h; i++)
        newdt[i] = new Color[w];
    return newdt;
}

/* Libera a matriz Color da memoria
   @param h: altura do bmp, ou numero de linhas da matriz
   @param w: largura do bmp, ou numero de colunas da matriz
*/
void Bmp::deleteBitmap(Color** dt, int h, int w)
{
    for(int i = 0; i < h; i++)
        delete dt[i];
    delete dt;
}

/* Renderiza/desenha a imagem bmp
   @param px: coordenada x onde o bmp comeca a ser desenhado
   @param py: coordenada y onde o bmp comeca a ser desenhado
*/
void Bmp::renderBmp()
{
    Vector2 mid = position / 2;
    Vector2 oldP = position;
    for(int i = 0; i < height; i++)
        for(int j = 0; j < width; j++) {
            vector<float> rgb = Utils::RGBtoFloat(dt[i][j].r , dt[i][j].g , dt[i][j].b);
            // rotacao se houver
            double oldX = j / mid.x;
            double oldY = i / mid.y;
            //Calcula os novos pontos pos rotacao da imagem
            int newY = round(-oldX * sin(rad) + oldY * cos(rad));
            int newX = round(oldX * cos(rad) + oldY * sin(rad));
            //Recentralia a imagem ja rotacionada
            newY = mid.y - newY;
            newX = mid.x - newX;

            // cores sao multiplicadas por channel para visualizacao dos canais
            if (rgb[0] != 1 && rgb[1] != 1 && rgb[2] != 1) { // branco, cor de fundo
                Vector2 p = Utils::rotatePoint(oldP, mid, rad);
                CV::color(rgb[0], rgb[1], rgb[2]);
                CV::point(position.x + j, position.y + height - i);
                oldP = p;
                //CV::point(newX + position.x, newY + position.y);
            }
        }
}

/* Rotaciona a imagem bmp 90 graus no sentido horario ou antihorario
   @param clockwise: inteiro 0 ou 1, indica em qual sentido girar a imagem
*/
void Bmp::rotateImage (float rad, int clockwise)
{
    this->rad = rad;
}

/* Rotaciona a imagem bmp 90 graus no sentido horario
   @param temp: matriz com altura e largura trocados
*/
void Bmp::rotateRight (float rad, Color** temp)
{
    Vector2 mid = position / 2;
    for (int i = 0; i < height; i++) {
        int y = 0;
        for (int j = width-1; j >= 0; j--) {
            temp[y][i] = dt[i][j];
            y++;
        }
    }
    position = Utils::rotatePoint(position, mid, rad);
}

/* Rotaciona a imagem bmp 90 graus no sentido antihorario
   @param temp: matriz com altura e largura trocados
*/
void Bmp::rotateLeft(float rad, Color** temp)
{
    int y = height - 1;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            temp[j][y] = dt[i][j];
        }
        y--;
    }
}

/* Le o HEADER componente a componente devido ao problema de alinhamento de bytes
   Usando o comando fread sao lidos 16 bytes ao inves de 14
   @param fp: ponteiro para o arquivo a ser lido
*/
void Bmp::readHeader(FILE* fp)
{
    fread(&header.type,      sizeof(unsigned short int), 1, fp);
    fread(&header.size,      sizeof(unsigned int),       1, fp);
    fread(&header.reserved1, sizeof(unsigned short int), 1, fp);
    fread(&header.reserved2, sizeof(unsigned short int), 1, fp);
    fread(&header.offset,    sizeof(unsigned int),       1, fp);
}

/* Escreve o HEADER componente a componente com as informacoes do novo bmp gerado
   @param fp: ponteiro para o arquivo a ser escrito
*/
void Bmp::writeHeader(FILE* fp)
{
    fwrite(&header.type,      sizeof(unsigned short int), 1, fp);
    fwrite(&header.size,      sizeof(unsigned int),       1, fp);
    fwrite(&header.reserved1, sizeof(unsigned short int), 1, fp);
    fwrite(&header.reserved2, sizeof(unsigned short int), 1, fp);
    fwrite(&header.offset,    sizeof(unsigned int),       1, fp);
}

/* Le o INFOHEADER componente a componente devido ao problema de alinhamento de bytes
   @param fp: ponteiro para o arquivo a ser lido
*/
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

/* Escreve o INFOHEADER componente a componente devido ao problema de alinhamento de bytes
   @param fp: ponteiro para o arquivo a ser escrito
*/
void Bmp::writeInfoHeader(FILE* fp)
{
    fwrite(&info.size,        sizeof(unsigned int),       1, fp);
    fwrite(&info.width,       sizeof(int),                1, fp);
    fwrite(&info.height,      sizeof(int),                1, fp);
    fwrite(&info.planes,      sizeof(unsigned short int), 1, fp);
    fwrite(&info.bits,        sizeof(unsigned short int), 1, fp);
    fwrite(&info.compression, sizeof(unsigned int),       1, fp);
    fwrite(&info.imagesize,   sizeof(unsigned int),       1, fp);
    fwrite(&info.xresolution, sizeof(int),                1, fp);
    fwrite(&info.yresolution, sizeof(int),                1, fp);
    fwrite(&info.ncolours,    sizeof(unsigned int),       1, fp);
    fwrite(&info.impcolours,  sizeof(unsigned int),       1, fp);
}

/* Carrega todas as informacoes do arquivo bmp nas estruturas da classe
   @param path: caminho que a imagem se encontra
*/
void Bmp::load(const char* path)
{
    FILE *fp = fopen(path, "rb");
    if(fp == NULL) {
        (*alert)->alerts.push_back(new Alert(350, 300, 500, 200, "Erro ao abrir arquivo", Utils::ERRO, TRUE));
        return;
    }
    readHeader(fp);
    readInfoHeader(fp);
    width  = info.width;
    height = info.height;
    bits   = info.bits;
    bytesPerLine =(3 * (width + 1) / 4) * 4;
    imagesize    = bytesPerLine*height;
    int padding  = (4 - (width * sizeof(Color)) % 4) % 4;

    if( header.type != 19778 ) {
        (*alert)->alerts.push_back(new Alert(350, 300, 500, 200, "Arquivo BMP invalido ", Utils::ERRO, TRUE));
        return;
    }
    if( info.compression != 0 ) {
        cout << "Compreensao nao suportada " << info.compression << endl;
        return;
    }
    if( bits != 24 ) {
        cout << "Formato BMP nao suporta esse numero de bits " << bits  << endl;
        return;
    }
    if( info.planes != 1 ) {
        cout << "Numero de Planes nao suportado " << endl;
        return;
    }

    dt = newBitmap(height, width);
    Color(*image)[width] = (Color(*)[width])calloc(height, width * sizeof(Color));
    for (int i = 0; i < height; i++) {
        fread(image[i], sizeof(Color), width, fp); // le toda a linha
        fseek(fp, padding, SEEK_CUR); // pula bytes extras, se houver
    }
    for(int i = 0; i < height; i++) // preenche o dt com as cores RGB da imagem
        for(int j = 0; j < width; j++)
            dt[i][j] = image[i][j];
    fclose(fp);
}
