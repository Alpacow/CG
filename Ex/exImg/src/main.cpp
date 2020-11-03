#include <GL/glut.h>
#include <GL/freeglut_ext.h> //callback da wheel do mouse.

#include <math.h>
#include <iostream>
#include <stdlib.h>
#include <vector>

#include "gl_canvas2d.h"
#include "Utils.h"
#include "Botao.h"
#include "Bmp.h"


#define RECT_SIZE 10
#define TEXT_COORD 2
#define MAX_TEXTURES 1

using namespace std;

Button   *bt = NULL; //se a aplicacao tiver varios botoes, sugiro implementar um manager de botoes.
Button   *bt2 = NULL; //se a aplicacao tiver varios botoes, sugiro implementar um manager de botoes.
vector<float> rgb; // controla cores passadas em RGB
// imagem
Bmp *img1;
unsigned char *data;
GLuint textureID[1];
float ang = 0;

int W = 800, H = 620;
//variavel global para selecao do que sera exibido na canvas.
int opcao  = 50;
int mouseX, mouseY; //variaveis globais do mouse para poder exibir dentro da render().


void DrawMouseScreenCoords()
{
    char str[100];
    sprintf(str, "Mouse: (%d,%d)", mouseX, mouseY);
    CV::text(10,300, str);
    sprintf(str, "Screen: (%d,%d)", W, H);
    CV::text(10,320, str);
}

//funcao chamada continuamente. Deve-se controlar o que desenhar por meio de variaveis globais
//Todos os comandos para desenho na canvas devem ser chamados dentro da render().
//Deve-se manter essa função com poucas linhas de codigo.
void render()
{
    CV::text(W/2 - 50, 10, "Teste imagens");
    bt->Render();
    bt2->Render();
    DrawMouseScreenCoords();
    if (opcao == 200)
        img1->setFlip(0);
    if (opcao == 202)
        img1->setFlip(1);
    img1->mirroredX();
    img1->renderBmp(250, 200);
}

//funcao chamada toda vez que uma tecla for pressionada.
void keyboard(int key)
{
    printf("\nTecla: %d", key);
    //if( key < 200)
    opcao = key;

    switch(key) {
    case 27:
        exit(0);
        break;
    }
}

//funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key)
{
    cout << "\nLiberou: " << key << endl;
}

//funcao para tratamento de mouse: cliques, movimentos e arrastos
void mouse(int button, int state, int wheel, int direction, int x, int y)
{
    mouseX = x; //guarda as coordenadas do mouse para exibir dentro da render()
    mouseY = y;
    //printf("\nmouse %d %d %d %d %d %d", button, state, wheel, direction,  x, y);
    if( state == 0 ) {
        if( bt->Colidiu(x, y) ) {
            cout << "Clicou no botao" << endl;
            img1->mirroredX();
        }
    }
}


int main(int argc, char *argv[])
{
    CV::init(&W, &H, "Ex 1: imagens");

    img1 = new Bmp("C:\\Users\\Flan\\Desktop\\CG\\Ex\\exImg\\resources\\img1.bmp");
    img1->convertBGRtoRGB();

    rgb = RGBtoFloat(70,130,180);
    CV::clear(rgb[0], rgb[1], rgb[2]);

    rgb = RGBtoFloat(222,184,135);
    bt = new Button(10, 20, 140, 50, (char*)"b1", rgb);
    bt2 = new Button(180, 20, 140, 50, (char*)"b2", rgb);
    CV::run();
}
