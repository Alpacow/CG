//****************************************************************
//
// classe derivada de Widget que controla os botoes da aplicacao
//
//****************************************************************
#include "Gear.h"
#include "../Canvas/gl_canvas2d.h"
#include "../Utils/Utils.h"

#define DIST 300

using namespace std;

/* Inicia todos os atributos necessarios
*/
Gear::Gear() {}

Gear::~Gear() {}

/* Inicia todos os atributos necessarios
   @param x: coordenada x do canto superior esquerdo que o alert comeca a ser desenhado
   @param y: coordenada y do canto superior esquerdo que o alert comeca a ser desenhado
   @param w: largura da janela
   @param h: altura da janela
   @param bg: array contendo a cor RGB do fundo do botao
   @param label: texto contido dentro do botao
   @param labelColor: array contendo a cor RGB do texto do botao
*/
Gear::Gear(float rad, float angTeeth, int nTeeth, int nFaces, vector<float> color)
{
    this->radius = rad;
    this->radiusOut = rad * 1.5;
    this->angTeeth = angTeeth;
    this->nTeeth = nTeeth;
    this->nFaces = nFaces;
    this->color = color;
    this->x = 0; // coordenadas iniciais
    this->y = 0;
    this->z = 300;
    this->width = 20;

    this->origin = Vector3{500, 300, 0};
    float increment = PI_2 / (float)nTeeth;
    initDraw2D(0, increment, origin.x, origin.y, radius, 1); // parte da frente da engrenagem
    initDraw2D(increment / 2.0, increment, origin.x, origin.y, radiusOut, 1);

    initDraw2D(0, increment, origin.x, origin.y, radius, 0); // parte de tras da engrenagem
    initDraw2D(increment / 2.0, increment, origin.x, origin.y, radiusOut, 0);
    for(vector<Vector3>::size_type i = 0; i != points.size(); i++)
        points2D.push_back(Vector3(points[i].x * DIST / points[i].z, points[i].y * DIST / points[i].z, 0));
    for(vector<Vector3>::size_type i = 0; i != lines.size(); i++)
        lines2D.push_back(Vector3(lines[i].x * DIST / lines[i].z, lines[i].y * DIST / lines[i].z, 0));
}

/* Renderiza/desenha o botao na tela
*/
void Gear::render()
{
    drawGear2D();
}

Vector3 Gear::calcToothPosition(float ang, float radius, float z, float tx, float ty)
{
    Vector3 coord;
    coord.x = tx + radius * cos(ang);
    coord.y = ty + radius * sin(ang);
    coord.z = z;
    return coord;
}

void Gear::drawGear2D()
{

    CV::color(color[0], color[1], color[2]);
    for(vector<Vector3>::size_type i = 0; i != points2D.size(); i++) {
        CV::point(points2D[i].x, points2D[i].y);
    }
    for(unsigned int i = 0; i < lines2D.size() - 1; i += 2) {
        CV::line(lines2D[i].x, lines2D[i].y, lines2D[i + 1].x, lines2D[i + 1].y);
    }

    vector<Vector3>::const_iterator first = lines2D.begin();
    vector<Vector3>::const_iterator last = lines2D.begin() + lines2D.size() / 2;
    vector<Vector3> frontPoints(first, last);
    first = lines2D.begin() + lines2D.size() / 2;
    last = lines2D.begin() + lines2D.size();
    vector<Vector3> backPoints(first, last);
    for(unsigned int i = 0; i < frontPoints.size() - 1; i++) {
      CV::line(frontPoints[i].x, frontPoints[i].y, backPoints[i].x, backPoints[i].y);
    }
    //CV::circle(origin.x * DIST / (z - width / 2.0), origin.y * DIST / (z - width / 2.0), radius - radius / 1.5, 50); // frente
    //CV::circle(origin.x * DIST / (z + width / 2.0), origin.y * DIST / (z + width / 2.0), radius - radius / 1.5, 50); // tras
}

void Gear::initDraw2D(float iniPoint, float increment, float translationX, float translationY, float rad, int frontBack)
{
    float p = 0;
    float w = width / 2.0;
    for (float t = iniPoint; t < PI_2; t += increment) {
        Vector3 p1 = calcToothPosition(t, radius, z, translationX, translationY);
        Vector3 p2 = calcToothPosition(t, radiusOut, z, translationX, translationY);
        if (frontBack) { // true = front
            lines.push_back(Vector3{p1.x, p1.y, p1.z - w});
            lines.push_back(Vector3{p2.x, p2.y, p2.z - w});
        } else {
            lines.push_back(Vector3{p1.x, p1.y, p1.z + w});
            lines.push_back(Vector3{p2.x, p2.y, p2.z + w});
        }
        for (p = t; p < (t + increment / 2.0); p += 0.001) {
            Vector3 p3 = calcToothPosition(p, rad, z, translationX, translationY);
            if (frontBack) // true = front
                points.push_back(Vector3{p3.x, p3.y, p3.z - w});
            else
                points.push_back(Vector3{p3.x, p3.y, p3.z + w});
        }
    }
}

void Gear::draw2D()
{
}
