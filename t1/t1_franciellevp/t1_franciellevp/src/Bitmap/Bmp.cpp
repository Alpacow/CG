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

Bmp::Bmp(){}

/* Inicializa atributos necessarios
   @param _fileName: nome da imagem bmp
   @param alerts: instancia da classe Alert para manipular janelas de mensagem
*/
Bmp::Bmp(string _path, Alert** alerts)
{
    width = height = 0;
    channel = {1,1,1,0};
    alert = alerts;
    path = _path;
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

/* Espelha a imagem bmp horizontalmente, trocando as colunas da matriz de lugar
*/
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

/* Espelha a imagem bmp verticalmente, trocando as linhas da matriz de lugar
*/
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

/* Rotaciona a imagem bmp 90 graus no sentido horario ou antihorario
   @param clockwise: inteiro 0 ou 1, indica em qual sentido girar a imagem
*/
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

/* Rotaciona a imagem bmp 90 graus no sentido horario
   @param temp: matriz com altura e largura trocados
*/
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

/* Rotaciona a imagem bmp 90 graus no sentido antihorario
   @param temp: matriz com altura e largura trocados
*/
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

/* Aplica luminancia na imagem bmp
*/
void Bmp::luminance ()
{
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++) {
            float luminance = .299 * dt[i][j].r + .587 * dt[i][j].g + .114 * dt[i][j].b;
            dt[i][j].r = luminance;
            dt[i][j].g = luminance;
            dt[i][j].b = luminance;
        }
}

/* Aplica o efeito sepia (envelhecimento) na imagem
*/
void Bmp::sepiaEffect ()
{
    for (int i = 0; i < height; i++)
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

/* Utiliza o metodo "blur box" para borrar a imagem
*  Para cada pixel faz uma media de todos seus vizinhos, incluindo a si proprio
*/
void Bmp::blurEffect()
{
    int sumBlue, sumGreen, sumRed;
    float countColor;
    Color temp[height][width]; // para nao ir alterando os valores de dt no loop

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            sumBlue = sumGreen = sumRed = 0;
            countColor = 0.00;
            for (int k = -1; k < 2; k++) {
                if (j + k < 0 || j + k > height - 1) continue; // se  vizinho "sai" da imagem
                for (int h = -1; h < 2; h++) {
                    if (i + h < 0 || i + h > width - 1) continue;// se vizinho "sai" da imagem
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

/* Diminui a escala da imagem pela metade
*/
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
    int delta    = bytesPerLine - (3 * width);
    int padding  = (4 - (width * sizeof(Color)) % 4) % 4;

    cout << "Imagem: " << width << "x" << height << " - Bits: " << bits << endl;
    cout << "bytesPerLine: " << bytesPerLine << endl;
    cout << "bytesPerLine: " << width * 3 << endl;
    cout << "delta: " << delta << endl;
    cout << "imagesize: " << imagesize << " " << info.imagesize << endl;

    if( header.type != 19778 ) {
        (*alert)->alerts.push_back(new Alert(350, 300, 500, 200, "Arquivo BMP invalido ", Utils::ERRO, TRUE));
        return;
    }
    if( width*height*3 != imagesize )
        (*alert)->alerts.push_back(new Alert(350, 300, 500, 200, "Arquivo BMP nao tem largura multipla de 4", Utils::WARNING, TRUE));
    if( info.compression != 0 ) {
        (*alert)->alerts.push_back(new Alert(350, 300, 500, 200, "Formato BMP comprimido nao suportado", Utils::ERRO, TRUE));
        return;
    }
    if( bits != 24 ) {
        (*alert)->alerts.push_back(new Alert(350, 300, 500, 200, "Formato BMP nao suporta esse numero de bits", Utils::ERRO, TRUE));
        return;
    }
    if( info.planes != 1 ) {
        (*alert)->alerts.push_back(new Alert(350, 300, 500, 200, "Numero de Planes nao suportado ", Utils::ERRO, TRUE));
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

/* Escreve os dados alterados (ou nao) da imagem em um novo bmp, adicionando "Save" ao nome
*/
void Bmp::write()
{
    (*alert)->alerts.push_back(new Alert(350, 300, 500, 200, "Salvando imagem", Utils::INFO, TRUE));
    path = path.substr(0, path.find(".bmp")) + "Save.bmp";
    const char* name = path.c_str();
    FILE *fp = fopen(name, "wb");
    info.width = width;
    info.height = height;
    info.bits = bits;
    int padding  = (4 - (width * sizeof(Color)) % 4) % 4;
    header.size = 54 + (width * height * 3) + (padding *3);
    writeHeader(fp);
    writeInfoHeader(fp);

    bytesPerLine =(3 * (width + 1) / 4) * 4;
    imagesize    = bytesPerLine*height;

    Color(*image)[width] = (Color(*)[width])calloc(height, width * sizeof(Color));
    for(int i = 0; i < height; i++) // preenche o dt com as cores RGB da imagem
        for(int j = 0; j < width; j++) {
            image[i][j].r = dt[i][j].r * channel[0];
            image[i][j].g = dt[i][j].g * channel[1];
            image[i][j].b = dt[i][j].b * channel[2];
        }

    for (int i = 0; i < height; i++) {
        fwrite(image[i], sizeof(Color), width, fp); // escreve toda a linha
        fseek(fp, padding, SEEK_CUR); // pula bytes extras, se houver
    }
    for (int k = 0; k < padding; k++)
            fputc(0x00, fp);
    fclose(fp);
}
