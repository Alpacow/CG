#include <GL/glut.h>
#include <GL/freeglut_ext.h> //callback da wheel do mouse.

#include "gl_canvas2d.h"
#include "Utils/Utils.h"
#include "Button/Button.h"
#include "Bitmap/Bmp.h"

#include <math.h>

#define RECT_SIZE 10
#define TEXT_COORD 2
#define MAX_TEXTURES 1
using namespace std;

int screenWidth = 1200, screenHeight = 1000;
Button* bts = new Button(); // controla todos os botões do programa
vector<float> rgb; // controla cores passadas em RGB
Bmp* img; // imagem a ser manipulada

int opcao  = 50; //variavel global para selecao do que sera exibido na canvas.
int mouseX, mouseY; //variaveis globais do mouse para poder exibir dentro da render().

//funcao chamada continuamente. Deve-se controlar o que desenhar por meio de variaveis globais
//Todos os comandos para desenho na canvas devem ser chamados dentro da render().
//Deve-se manter essa função com poucas linhas de codigo.
void render()
{
    CV::text(screenWidth/2 - 80, 20, "T1 - Francielle Vasconcellos Pereira");
    bts->renderButtons();
    if (opcao == 200)
        img->setFlip(0);
    if (opcao == 202)
        img->setFlip(1);
    img->renderBmp(250, 200);
}

//funcao chamada toda vez que uma tecla for pressionada.
void keyboard(int key)
{
    cout << endl << "Tecla: " << key << endl;
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
    bts->checkButtonState(state, x, y);
}


int main(int argc, char *argv[])
{
    CV::init(&screenWidth, &screenHeight, "T1 - Visualizador de Imagens");
    img = new Bmp(Utils::getPath("img1.bmp"));
    img->convertBGRtoRGB();
    rgb = Utils::RGBtoFloat(28,28,28);
    CV::clear(rgb[0], rgb[1], rgb[2]);

    CV::run();
}
