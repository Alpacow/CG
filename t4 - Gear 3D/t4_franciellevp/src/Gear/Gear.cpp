//****************************************************************
//
// classe responsavel pelo desenho e transformacoes da engrenagem
//
//****************************************************************
#include "Gear.h"
#include "../Canvas/gl_canvas2d.h"
#include "../Utils/Utils.h"

using namespace std;

/* Inicia todos os atributos necessarios
*/
Gear::Gear() {}

Gear::~Gear() {}

/* Inicia todos os atributos necessarios
   @param rad: raio inicial da parte da frente da engrenagem
   @param radBack: raio inicial da parte da tras da engrenagem
   @param nTeeth: nro de dentes
   @param color: cor RGB deseada
   @param x: coordenada x do centro da engrenagem
   @param y: coordenada y do centro da engrenagem
   @param z: coordenada z do centro da engrenagem
*/
Gear::Gear(float rad, float radBack, int nTeeth, vector<float> color, float x, float y, float z)
{
    this->radius = rad;
    this->radiusBack = radBack;
    this->nTeeth = nTeeth;
    this->color = color;
    this->width = 2;
    this->widthP = 2;
    this->screenDist = 200;
    this->velRotation = 1;
    this->velTranslation = 1;
    this->teethSize = 1.5;
    this->origin = Vector3{0, 0, 0};
    this->translation = Vector3{x, y, z};
    this->rotateX = this->rotateY = this->perspective = false;
    this->rotateZ = true;
    this->orthographic = true;

    updateArraysSize();
    orthographicDraw();
    initGear();
}

/* Renderiza/desenha a engrenagem na tela
*/
void Gear::render(float fps)
{
    if (rotateZ)
        rotate3D(Utils::Z, velRotation * PI / 180);
    if (rotateY)
        rotate3D(Utils::Y, velRotation * PI / 180);
    if (rotateX)
        rotate3D(Utils::X, velRotation * PI / 180);
    if (orthographic || perspective)
        drawGear2D();
}

/* Calcula a posicao do dente na engrenagem no circulo
   @param ang: angulo em radianos
   @param radius: radio do circulo
   @param z: coordenada do eixo z
   @return: vetor com as 3 coordenadas do ponto
*/
Vector3 Gear::calcToothPosition(float ang, float radius, float z)
{
    Vector3 coord;
    coord.x = origin.x + radius * cos(ang);
    coord.y = origin.y + radius * sin(ang);
    coord.z = z;
    return coord;
}

/* Desenha a engrenagem 2D com perspectiva
*/
void Gear::drawGear2D()
{
    // projecao
    for (vector<Vector3>::size_type i = 0; i != points.size(); i++)
        points2D.at(i) = Vector3{points[i].x * screenDist / (points[i].z + translation.z), points[i].y * screenDist / (points[i].z  + translation.z), 0};
    for (vector<Vector3>::size_type i = 0; i != lines.size(); i++)
        lines2D.at(i) = Vector3{lines[i].x * screenDist / (lines[i].z + translation.z), lines[i].y * screenDist / (lines[i].z + translation.z), 0};

    // translada para o ponto desejado
    for (vector<Vector3>::size_type i = 0; i != points2D.size(); i++)
        points2D.at(i) += translation;
    for (vector<Vector3>::size_type i = 0; i != lines2D.size(); i++)
        lines2D.at(i) += translation;

    // desenha
    CV::color(color[0], color[1], color[2]);
    for (vector<Vector3>::size_type i = 0; i != points2D.size(); i++)
        CV::point(points2D[i].x, points2D[i].y);
    for (unsigned int i = 0; i < lines2D.size() - 1; i += 2)
        CV::line(lines2D[i], lines2D[i + 1]);

    // linhas que ligam os dentes da parte de fora
    for (unsigned int i = 2; i < lines2D.size() / 2.0 - 2; i += 4)
        CV::line(lines2D[i], lines2D[i + 3]);
    for (unsigned int i = lines2D.size() / 2.0 + 2; i < lines2D.size() - 2; i += 4)
        CV::line(lines2D[i], lines2D[i + 3]);

    // liga o primeiro com ultimo ponto da parte de frente e de tras da engrenagem
    CV::line(lines2D[lines2D.size() / 2.0 - 2], lines2D[1]);
    CV::line(lines2D[lines2D.size() - 2], lines2D[lines2D.size() / 2.0 + 1]);

    // linhas que ligam a parte de frente com parte de tras
    for (unsigned int i = 0; i < lines2D.size() / 2.0; i++)
        CV::line(lines2D[i], lines2D[i + lines2D.size() / 2.0]);
}

/* Inicia os valores dos arrays para desenho da engrenagem
   @param i: posicao do indice no vector de linhas
   @param j: posicao do indice no vector de pontos
   @param frontBack: booleano que indica se esta calculando a parte da frente ou de tras da engrenagem
   @param radiusInOut: o raio da frente ou de tras da engrenagem
*/
void Gear::initDraw2D(int* i, int* j, bool frontBack, float radiusFrontBack)
{
    angTeeth = PI_2 / (float)nTeeth;
    float radiusOut = radiusFrontBack * teethSize;
    float centerRadius = radius - radius / 1.5;
    float centerRadiusBack = radiusBack - radiusBack / 1.5;
    float p, w = width / 2.0;
    bool inOut = true;
    Vector3 p1, p2, p3;
    for (float t = 0; t <= PI_2; t += (angTeeth / 2.0)) {
        if (inOut) {
            p1 = calcToothPosition(t, radiusFrontBack, origin.z);
            p2 = calcToothPosition(t, radiusOut, origin.z);
            for (p = t; p < (t + angTeeth / 2.0); p += 0.001) {
                p3 = calcToothPosition(p, radiusFrontBack, origin.z);
                if (frontBack) // true = front
                    points[*j] = Vector3{p3.x, p3.y, p3.z - w};
                else
                    points[*j] = Vector3{p3.x, p3.y, p3.z + w};
                *j += 1;
            }
        } else {
            p1 = calcToothPosition(t, radiusOut, origin.z);
            p2 = calcToothPosition(t, radiusFrontBack, origin.z);
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
    for (float t = 0; t <= PI_2; t += 0.01) {
        p1 = calcToothPosition(t, centerRadius, origin.z - w);
        points[*j] = p1;
        *j += 1;
        p2 = calcToothPosition(t, centerRadiusBack, origin.z + w);
        points[*j] = p2;
        *j += 1;
    }
}

/* Rotaciona todos os pontos necessarios para o desenho da engrenagem
   @param axis: indica qual eixo deseja-se rotacionar (x, Y, Z)
   @param rad: angulo a se rotacionar os pontos em radianos
*/
void Gear::rotate3D(int axis, float rad)
{
    for(vector<Vector3>::size_type i = 0; i != points.size(); i++)
        points[i] = Utils::rotatePoint(points[i], rad, axis);
    for(unsigned int i = 0; i < lines.size(); i++)
        lines[i] = Utils::rotatePoint(lines[i], rad, axis);
}

/* Move a engrenagem no eixo Z
   @param dist: distancia a ser transladada
*/
void Gear::MoveZ (float dist)
{
    origin.z += (velTranslation * dist);
    for (vector<Vector3>::size_type i = 0; i != points.size(); i++)
        points[i].z += (velTranslation * dist);
    for (vector<Vector3>::size_type i = 0; i != lines.size(); i++)
        lines[i].z += (velTranslation * dist);
}

/* Move a engrenagem no eixo Y
   @param dist: distancia a ser transladada
*/
void Gear::MoveY (float dist)
{
    origin.y += (velTranslation * dist);
    for (vector<Vector3>::size_type i = 0; i != points.size(); i++)
        points[i].y += (velTranslation * dist);
    for (vector<Vector3>::size_type i = 0; i != lines.size(); i++)
        lines[i].y += (velTranslation * dist);
}

/* Move a engrenagem no eixo X
   @param dist: distancia a ser transladada
*/
void Gear::MoveX (float dist)
{
    origin.x += (velTranslation * dist);
    for (vector<Vector3>::size_type i = 0; i != points.size(); i++)
        points[i].x += (velTranslation * dist);
    for (vector<Vector3>::size_type i = 0; i != lines.size(); i++)
        lines[i].x += (velTranslation * dist);
}

/* Metodo auxiliar para desenhar as partes frontal e traseira da engrenagem
*/
void Gear::initGear ()
{
    int i = 0, j = 0;
    initDraw2D(&i, &j, 1, radius); // parte da frente da engrenagem
    initDraw2D(&i, &j, 0, radiusBack); // parte da tras da engrenagem
}

/* Atualiza o tamanho dos vetores dos pontos da engrenagem
*/
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
    for (float t = 0; t <= PI_2; t += 0.01)
        countp += 2;
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

/* Seta a "largura" da engrenagem
   @param value: novo valor de largura
*/
void Gear::setWidth (float value)
{
    if (perspective && !orthographic) {
        this->widthP = value;
        this->width = value;
    } else {
        this->widthP = value;
    }
    initGear();
}

/* Seta o numero de dentes
   @param value: nova quantidade de dentes
*/
void Gear::setNroTeeth (int value)
{
    this->nTeeth = value;
    updateArraysSize();
    initGear();
}

/* Seta o raio da frente da engrenagem e atualiza as variaveis necessarias caso a visualizacao seja ortografica
   @param value: novo valor do raio
*/
void Gear::setRadius (float value)
{
    if (perspective) {
        this->radius = value;
    } else {
        this->radius = value;
        this->radiusBack = value;
    }
    initGear();
}

/* Seta o raio de tras da engrenagem e atualiza as variaveis necessarias caso a visualizacao seja ortografica
   @param value: novo valor do raio
*/
void Gear::setRadiusBack (float value)
{
    if (perspective) {
        this->radiusBack = value;
    } else {
        this->radiusBack = value;
        this->radius = value;
    }
    initGear();
}

/* Seta a variavel de controle que indica se deve rotacionar o eixo Z
   @param value: novo valor da variavel de controle
*/
void Gear::setRotateZ (bool value)
{
    this->rotateZ = value;
}

/* Seta a variavel de controle que indica se deve rotacionar o eixo Y
   @param value: novo valor da variavel de controle
*/
void Gear::setRotateY (bool value)
{
    this->rotateY = value;
}

/* Seta a variavel de controle que indica se deve rotacionar o eixo X
   @param value: novo valor da variavel de controle
*/
void Gear::setRotateX (bool value)
{
    this->rotateX = value;
}

/* Seta o tamanho dos dentes da engrenagem
   @param value: novo valor do tamanho
*/
void Gear::setTeethSize (float value)
{
    this->teethSize = value;
    initGear();
}

/* Atualiza a largura e faz os raios terem o mesmo valor quando a visualizacao for ortografica
*/
void Gear::orthographicDraw ()
{
    if (orthographic) {
        this->width = 0;
        this->radiusBack = radius;
    }
    initGear();
}

/* Atualiza a largura quando a visualizacao for ortografica
*/
void Gear::perspectiveDraw ()
{
    if (perspective)
        this->width = this->widthP;
    initGear();
}
