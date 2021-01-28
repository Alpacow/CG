//****************************************************************
//
// classe derivada de Widget que controla os botoes da aplicacao
//
//****************************************************************
#include "Gear.h"
#include "../Canvas/gl_canvas2d.h"
#include "../Utils/Utils.h"

#define IDX_POINTS 6290
//8384
#define IDX_LINES 40

#define MAX_X 1000

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
    this->width = 10;
    this->screenDist = 300;
    this->velRotation = 1;
    this->velTranslation = 1;
    this->origin = Vector3{0, 0, 300};
    this->translation = Vector3{x, y, z};
    this->angTeeth = PI_2 / (float)nTeeth;

    updateArraysSize();
    initGear();
}

/* Renderiza/desenha o botao na tela
*/
void Gear::render()
{
    rotate3D(Utils::Z, velRotation * PI / 180);
    drawGear2D();
}

Vector3 Gear::calcToothPosition(float ang, float radius, float z)
{
    Vector3 coord;
    coord.x = origin.x + radius * cos(ang);
    coord.y = origin.y + radius * sin(ang);
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
        points2D.at(i) += translation;
    for (vector<Vector3>::size_type i = 0; i != lines2D.size(); i++)
        lines2D.at(i) += translation;

    // desenha
    CV::color(color[0], color[1], color[2]);
    for (vector<Vector3>::size_type i = 0; i != points2D.size(); i++) {
        if (i < points2D.size() / 2.0) CV::color (1,0,0);
        else CV::color (0,1,0);
        CV::point(points2D[i].x, points2D[i].y);
    }
    for (unsigned int i = 0; i < lines2D.size() - 1; i += 2) {
        if (i < lines2D.size() / 2.0) CV::color (1,0,0);
        else CV::color (0,1,0);
        CV::line(lines2D[i], lines2D[i + 1]);
    }

    // linhas que ligam os dentes da parte de fora
    for (unsigned int i = 2; i < lines2D.size() / 2.0 - 2; i += 4) {
        if (i < points2D.size() / 2.0 - 2) CV::color (1,0,0);
        else CV::color (0,1,0);
        CV::line(lines2D[i], lines2D[i + 3]);
    }
    for (unsigned int i = lines2D.size() / 2.0 + 2; i < lines2D.size() - 2; i += 4) {
        if (i < lines2D.size() / 2.0) CV::color (1,0,0);
        else CV::color (0,1,0);
        CV::line(lines2D[i], lines2D[i + 3]);
    }

    // liga o primeiro com ultimo ponto da parte de frente e de tras da engrenagem
    CV::color (1,0,0);
    CV::line(lines2D[lines2D.size() / 2.0 - 2], lines2D[1]);
    CV::color(0,1,0);
    CV::line(lines2D[lines2D.size() - 2], lines2D[lines2D.size() / 2.0 + 1]);

    // linhas que ligam a parte de frente com parte de tras
    for (unsigned int i = 0; i < lines2D.size() / 2.0; i++)
        CV::line(lines2D[i], lines2D[i + lines2D.size() / 2.0]);
}

void Gear::initDraw2D(int* i, int* j, bool frontBack)
{
    angTeeth = PI_2 / (float)nTeeth;
    float p, w = width / 2.0;
    int countp = 0, countl = 0;
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
                countp++;
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
        countl += 2;
        *i += 2;
        inOut = !inOut;
    }
}

void Gear::rotate3D(int axis, float rad)
{
    for(vector<Vector3>::size_type i = 0; i != points.size(); i++) {
        //points[i] = Utils::translate(points[i], Vector3{-origin.x, -origin.y, -1});
        points[i] = Utils::rotatePoint(points[i], rad, axis);
        //points[i] = Utils::translate(points[i], Vector3{origin.x, origin.y, 1});
    }
    for(unsigned int i = 0; i < lines.size(); i++) {
        //lines[i] = Utils::translate(lines[i], Vector3{-origin.x, -origin.y, -1});
        lines[i] = Utils::rotatePoint(lines[i], rad, axis);
        //lines[i] = Utils::translate(lines[i], Vector3{origin.x, origin.y, 1});
    }
}

void Gear::MoveZ (float dist)
{
    for (vector<Vector3>::size_type i = 0; i != points.size(); i++)
        points[i].z += (velTranslation * dist);
    for (vector<Vector3>::size_type i = 0; i != lines.size(); i++)
        lines[i].z += (velTranslation * dist);
}

void Gear::MoveY (float dist)
{
    for (vector<Vector3>::size_type i = 0; i != points.size(); i++)
        points[i].y += (velTranslation * dist);
    for (vector<Vector3>::size_type i = 0; i != lines.size(); i++)
        lines[i].y += (velTranslation * dist);
}

void Gear::MoveX (float dist)
{
    for (vector<Vector3>::size_type i = 0; i != points.size(); i++)
        //if (points2D[i].x >= 0 && points2D[i].x < MAX_X)
        points[i].x += (velTranslation * dist);
    for (vector<Vector3>::size_type i = 0; i != lines.size(); i++)
        lines[i].x += (velTranslation * dist);
}

void Gear::initGear ()
{
    int i = 0, j = 0;
    initDraw2D(&i, &j, 1); // parte da frente da engrenagem
    initDraw2D(&i, &j, 0); // parte da tras da engrenagem
}

void Gear::updateArraysSize()
{
    int sizeLines = 0, sizePoints = 0;
    float angle = PI_2 / (float)nTeeth;
    float p;
    int countp = 0, countl = 0;
    bool inOut = true;
    for (float t = 0; t <= PI_2; t += (angle / 2.0)) {
        if (inOut)
            for (p = t; p < (t + angle / 2.0); p += 0.001)
                countp++;
        countl += 2;
        inOut = !inOut;
    }
    sizeLines = countl * 2;
    sizePoints = countp * 2;

    Utils::clearVector(lines);
    Utils::clearVector(lines2D);
    lines.reserve(sizeLines);
    lines2D.reserve(sizeLines);
    for (int i = 0; i < sizeLines; i++) {
        lines.push_back(Vector3{0, 0, 0});
        lines2D.push_back(Vector3{0, 0, 0});
    }
    Utils::clearVector(points);
    Utils::clearVector(points2D);
    points.reserve(sizePoints);
    points2D.reserve(sizePoints);
    for (int i = 0; i < sizePoints; i++) {
        points.push_back(Vector3{0, 0, 0});
        points2D.push_back(Vector3{0, 0, 0});
    }
}

void Gear::setWidth (float value)
{
    this->width = value;
    initGear();
}

void Gear::setNroTeeth (int value)
{
    this->nTeeth = value;
    updateArraysSize();
    initGear();
}
