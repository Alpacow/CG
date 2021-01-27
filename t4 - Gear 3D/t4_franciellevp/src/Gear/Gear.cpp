//****************************************************************
//
// classe derivada de Widget que controla os botoes da aplicacao
//
//****************************************************************
#include "Gear.h"
#include "../Canvas/gl_canvas2d.h"
#include "../Utils/Utils.h"

#define IDX_POINTS 6681
//13362
#define IDX_LINES 32
//68

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
Gear::Gear(float rad, int nTeeth, int nFaces, vector<float> color, float x, float y, float z)
{
    this->radius = rad;
    this->radiusOut = rad * 1.5;
    this->nTeeth = nTeeth;
    this->nFaces = nFaces;
    this->color = color;
    this->width = 20;
    this->screenDist = 300;
    this->origin = Vector3{x, y, z};
    this->inOutTurn = false;
    this->angTeeth = PI_2 / (float)nTeeth;
    this->radiusOffset = angTeeth * radius * 0.005; // inner taper offset (100% = half notch)
    this->radiusOutOffset = angTeeth * radiusOut * 0.005; // inner taper offset (100% = half notch)

    lines.reserve(IDX_LINES);
    lines2D.reserve(IDX_LINES);
    this->ang = angTeeth;
    for (unsigned int i = 0; i < IDX_LINES; i++) {
        lines.push_back(Vector3{0, 0, origin.z});
        lines2D.push_back(Vector3{0, 0, origin.z});
    }
    points.reserve(IDX_POINTS);
    points2D.reserve(IDX_POINTS);
    for (unsigned int i = 0; i < IDX_POINTS; i++) {
        points.push_back(Vector3{0, 0, origin.z});
        points2D.push_back(Vector3{0, 0, origin.z});
    }

    int i = 0, j = 0;
    initDraw2D(&i, &j, 1); // parte da frente da engrenagem
}

/* Renderiza/desenha o botao na tela
*/
void Gear::render()
{
    //rotate3D(Utils::Y, 1 * PI / 180);
    drawGear2D();
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
    // projecao
    for (vector<Vector3>::size_type i = 0; i != points.size(); i++)
        points2D.at(i) = Vector3{points[i].x * screenDist / points[i].z, points[i].y * screenDist / points[i].z, 0};
    for (vector<Vector3>::size_type i = 0; i != lines.size(); i++)
        lines2D.at(i) = Vector3{lines[i].x * screenDist / lines[i].z, lines[i].y * screenDist / lines[i].z, 0};

    // translada para o ponto desejado (origem do desenho)
    for (vector<Vector3>::size_type i = 0; i != points2D.size(); i++)
        points2D.at(i) += origin;
    for (vector<Vector3>::size_type i = 0; i != lines2D.size(); i++)
        lines2D.at(i) += origin;

    CV::color(color[0], color[1], color[2]);
    for (vector<Vector3>::size_type i = 0; i != points2D.size(); i++) {
        CV::point(points2D[i].x + origin.x, points2D[i].y + origin.y);
    }
    for (unsigned int i = 0; i < lines2D.size() - 1; i += 2) {
        CV::color(color[0], color[1], color[2]);
        CV::line(lines2D[i], lines2D[i + 1]);
    }
    for (unsigned int i = 2; i < lines2D.size() - 2; i += 4) {
        CV::line(lines2D[i], lines2D[i + 3]);
    }
    CV::line(lines2D[30], lines2D[1]);

    vector<Vector3>::const_iterator first = lines2D.begin();
    vector<Vector3>::const_iterator last = lines2D.begin() + lines2D.size() / 2;
    vector<Vector3> frontPoints(first, last);
    first = lines2D.begin() + lines2D.size() / 2;
    last = lines2D.begin() + lines2D.size();
    vector<Vector3> backPoints(first, last);
    for (unsigned int i = 0; i < frontPoints.size(); i++) {
      //CV::line(frontPoints[i].x + origin.x, frontPoints[i].y + origin.y, backPoints[i].x + origin.x, backPoints[i].y + origin.y);
    }
    //CV::circle(origin.x * DIST / (origin.z - width / 2.0), origin.y  * DIST / (origin.z - width / 2.0), radius - radius / 1.5, 50); // frente
    //CV::circle(origin.x * DIST / (origin.z + width / 2.0), origin.y * DIST / (origin.z + width / 2.0), radius - radius / 1.5, 50); // tras
}

void Gear::initDraw2D(int* i, int* j, bool frontBack)
{
    float p, w = width / 2.0;
    bool inOut = true;
    for (float t = 0; t <= PI_2; t += (angTeeth / 2.0)) {
        Vector3 p1, p2, p3;
        if (inOut) {
            p1 = calcToothPosition(t, radius, origin.z);
            p2 = calcToothPosition(t, radiusOut, origin.z);
            for (p = t; p < (t + angTeeth / 2.0); p += 0.001) {
                p3 = calcToothPosition(p, radius, origin.z);
                if (frontBack) // true = front
                    points[*j] = Vector3{p3.x, p3.y, p3.z - w};
                else
                    points[*j] = Vector3{p3.x, p3.y, p3.z + w};
                *j += 1;
            }
        } else {
            p1 = calcToothPosition(t, radiusOut, origin.z);
            p2 = calcToothPosition(t, radius, origin.z);
        }
        if (frontBack) { // true = front
            lines[*i] = Vector3{p1.x, p1.y, p1.z - w};
            lines[*i + 1] = Vector3{p2.x, p2.y, p2.z - w};
        } else {
            lines[*i] = Vector3{p1.x, p1.y, p1.z + w};
            lines[*i + 1] = Vector3{p2.x, p2.y, p2.z + w};
        }
        *i += 2;
        inOut = !inOut;
    }
}

void Gear::rotate3D(int axis, float rad)
{
    for (vector<Vector3>::size_type i = 0; i != points2D.size(); i++) {
        Utils::translate(points2D[i], Vector3{-origin.x, -origin.y, -origin.z});
        if (Utils::Z)
            points2D[i] = Utils::rotatePoint(points2D[i], rad, axis);
        Utils::translate(points2D[i], origin);
    }
    for (unsigned int i = 0; i < lines2D.size(); i++) {
        Utils::translate(lines2D[i], Vector3{-origin.x, -origin.y, -origin.z});
        lines2D[i] = Utils::rotatePoint(lines2D[i], rad, axis);
        Utils::translate(lines2D[i], origin);
    }
}

void Gear::MoveZ (float dist)
{
    origin.z += dist;
    for (vector<Vector3>::size_type i = 0; i != points.size(); i++)
        points[i].z += dist;
    for (vector<Vector3>::size_type i = 0; i != lines.size(); i++)
        lines[i].z += dist;
}

void Gear::MoveY (float dist)
{
    origin.y += dist;
}

void Gear::MoveX (float dist)
{
    origin.x += dist;
}
