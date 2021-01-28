

#include <GL/glut.h>
#include <GL/freeglut_ext.h> //callback da wheel do mouse.


#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "gl_canvas2d.h"

#include "Objeto.h"
#include "Cubo.h"
#include "paint.h"


//variaveis globais
int   opcao  = 50;
float global = 0;

Cubo* cubo;
float dist_to_screen = 600;
Paint* paint;

void render()
{
    color(0,0,0);
    //cubo->Draw3D(dist_to_screen);
    cubo->Draw2D(dist_to_screen);

    //paint->Draw();

}

//funcao chamada toda vez que uma tecla for pressionada
void keyboard(int key)
{
   printf("\nTecla: %d" , key);
   if(key == 43)
        dist_to_screen += 10;
   if(key == 45)
        dist_to_screen -= 10;

    if(key == 202)
        cubo->MoveZ(10);
   if(key == 200)
        cubo->MoveZ(-10);

    if(key == 201)
        cubo->MoveY(10);
   if(key == 203)
        cubo->MoveY(-10);

   if(key == 101)
        paint->Erase();

}

//funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key)
{
   printf("\nLiberou: %d" , key);

}

//funcao para tratamento de mouse: cliques, movimentos e arrastos
void mouse(int button, int state, int wheel, int direction, int x, int y)
{
   printf("\nmouse %d %d %d %d %d %d", button, state, wheel, direction,  x, 900-y);

   paint->AddPoint(x, 900 - y);

   if(button == 0 && state == 0){
        paint->Toggle();
   }

   if(button == 0 && state == 1){
        paint->Toggle();
   }
}

int main(void)
{
   initCanvas(1600,900, "Demo Câmera");

   cubo = new Cubo(0,0,300,100);
   paint = new Paint();

   runCanvas();
}
