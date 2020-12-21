#include <GL/glut.h>
#include <GL/freeglut_ext.h> //callback da wheel do mouse.

#include "Controller/Controller.h"
#include <iostream>

using namespace std;

Controller* cont = new Controller();

//funcao chamada continuamente. Deve-se controlar o que desenhar por meio de variaveis globais
//Todos os comandos para desenho na canvas devem ser chamados dentro da render().
//Deve-se manter essa função com poucas linhas de codigo.
void render()
{
    cont->Render();
}

//funcao chamada toda vez que uma tecla for pressionada.
void keyboard(int key)
{
    cont->Keyboard(key);
}

//funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key)
{
   //cout << "\nLiberou: " << key << endl;
}

//funcao para tratamento de mouse: cliques, movimentos e arrastos
void mouse(int button, int state, int wheel, int direction, int x, int y)
{
    cont->Mouse(button, x, y, state);
}

int main(int argc, char *argv[])
{
    cont->InitCanvas();
}
