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
    this->width = 20;
    this->origin = Vector3{500, 310, 300};
    this->translation = Vector3{500, 310, 0};

    float increment = PI_2 / (float)nTeeth;
    initDraw2D(0, increment, radius, 1); // parte da frente da engrenagem
    initDraw2D(increment / 2.0, increment, radiusOut, 1);


    initDraw2D(0, increment, radius, 0); // parte de tras da engrenagem
    initDraw2D(increment / 2.0, increment, radiusOut, 0);
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
    //rotate3D(Utils::X);
}

Vector3 Gear::calcToothPosition(float ang, float radius, float z)
{
    Vector3 coord;
    coord.x = radius * cos(ang);
    coord.y = radius * sin(ang);
    coord.z = z;
    return coord;
}

void Gear::drawGear2D()
{
    CV::color(color[0], color[1], color[2]);
    for(vector<Vector3>::size_type i = 0; i != points2D.size(); i++) {
        CV::point(points2D[i].x + origin.x, points2D[i].y + origin.y);
    }
    for(unsigned int i = 0; i < lines2D.size() - 1; i += 2) {
        CV::line(lines2D[i].x + origin.x, lines2D[i].y + origin.y, lines2D[i + 1].x + origin.x, lines2D[i + 1].y + origin.y);
    }

    vector<Vector3>::const_iterator first = lines2D.begin();
    vector<Vector3>::const_iterator last = lines2D.begin() + lines2D.size() / 2;
    vector<Vector3> frontPoints(first, last);
    first = lines2D.begin() + lines2D.size() / 2;
    last = lines2D.begin() + lines2D.size();
    vector<Vector3> backPoints(first, last);
    for(unsigned int i = 0; i < frontPoints.size(); i++) {
      //CV::line(frontPoints[i].x + origin.x, frontPoints[i].y + origin.y, backPoints[i].x + origin.x, backPoints[i].y + origin.y);
    }
    //CV::circle(origin.x * DIST / (origin.z - width / 2.0), origin.y  * DIST / (origin.z - width / 2.0), radius - radius / 1.5, 50); // frente
    //CV::circle(origin.x * DIST / (origin.z + width / 2.0), origin.y * DIST / (origin.z + width / 2.0), radius - radius / 1.5, 50); // tras
}

void Gear::initDraw2D(float iniPoint, float increment, float rad, int frontBack)
{
    float p = 0;
    float w = width / 2.0;
    for (float t = iniPoint; t < PI_2; t += increment) {
        Vector3 p1 = calcToothPosition(t, radius, origin.z);
        Vector3 p2 = calcToothPosition(t, radiusOut, origin.z);
        if (frontBack) { // true = front
            lines.push_back(Vector3{p1.x, p1.y, origin.z - w});
            lines.push_back(Vector3{p2.x, p2.y, origin.z - w});
        } else {
            lines.push_back(Vector3{p1.x, p1.y, origin.z + w});
            lines.push_back(Vector3{p2.x, p2.y, origin.z + w});
        }
        for (p = t; p < (t + increment / 2.0); p += 0.001) {
            Vector3 p3 = calcToothPosition(p, rad, origin.z);
            if (frontBack) // true = front
                points.push_back(Vector3{p3.x, p3.y, p3.z - w});
            else
                points.push_back(Vector3{p3.x, p3.y, p3.z + w});
        }
    }
}

void Gear::rotate3D(int axis)
{
    float rad = 0.2*3.1415/180;
    for(vector<Vector3>::size_type i = 0; i != points2D.size(); i++) {
        Utils::translate(points2D[i], Vector3{-0, -0, -0});
        if (Utils::Z)
            points2D[i] = Utils::rotatePoint(points2D[i], rad, axis);
        Utils::translate(points2D[i], Vector3{0, 0, 0});
    }
    for(unsigned int i = 0; i < lines2D.size(); i++) {
        Utils::translate(lines2D[i], Vector3{-0, -0, -0});
        lines2D[i] = Utils::rotatePoint(lines2D[i], rad, axis);
        Utils::translate(lines2D[i], Vector3{0, 0, 0});
    }
}
