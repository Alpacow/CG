//*****************************************************************
//
// classe para desenhar e controlar a pista responsavel pela pista
//
//*****************************************************************
#include "Bezier.h"
#include "../Utils/Utils.h"
#include "../Canvas/gl_canvas2d.h"
#include <iostream>

using namespace std;

/* Inicializa atributos necessarios
   @param alerts: instancia da classe Alert para manipular janelas de mensagem
*/
Bezier::Bezier(Alert** alert)
{
    this->alert = alert;
    cp = new ControlPoints();
    canApplyTransformations = translationMode = showHelp = raceOn = false;
    speedWayWidth = 40;
    scale = 0;
    center = Vector2 {0, 0};
    estimatedPoints.reserve(INDEX);
    for(unsigned int i = 0; i < INDEX; i++)
        estimatedPoints.push_back(Vector2 {0, 0});
    updateSpeedWaySize();
}

Bezier::~Bezier() {}

/* Retorna os pontos de controle da curva
   @return cp: pontos de controle
*/
ControlPoints* Bezier::getControlPoints()
{
    return cp;
}

/* Retorna os pontos estimados da curva de acordo com os pontos de controle
   @return estimatedPoints: pontos estimados da curva de Bezier
*/
vector<Vector2> Bezier::getEstimatedPoints()
{
    return estimatedPoints;
}

/* Renderiza/desenha tudo que sera necessario na tela
*/
void Bezier::render()
{
    if (!canApplyTransformations && !raceOn) {
        for(vector<ControlPoints>::size_type i = 0; i != cp->points.size(); i++)
            cp->points[i]->render();
        cp->drawControlGraph();
    }
    if (cp->points.size() > 2)
        drawBezierCurveForPolygon();
    if (cp->points.size() > 1)
        drawBezierCurve();
    if (canApplyTransformations) {
        if (translationMode) {
            CV::color(0,1,0);
            CV::circleFill(center.x, center.y, 5, 25);
        }
    }
    calculatePointsBezier();
}

/* Controla a posicao do mouse e se houve clique ou nao
   @param button: estado dos botoes do mouse
   @param x: coordenada x do mouse
   @param y: coordenada y do mouse
   @param state: verifica qual botao foi pressionado
*/
void Bezier::checkMouseStates(int button, int x, int y, int state)
{
    ControlPoints* cPoint = cp->checkCollisionDragPoints(x, y);
    if (button == 0) {
        if (state == 0) { // clicou no botao esquerdo
            if (cPoint == nullptr && !canApplyTransformations) // se nao colide com nenhum outro ponto
                cp->addPoint(x, y);
            else if (cPoint != nullptr) {
                cp->unsetAllPoints();
                cPoint->canDragPoint = true;
                cPoint->isSelect = true;
            }
        } else if (state == 1 && cPoint != nullptr)// soltou o mouse
            cPoint->canDragPoint = false;
    } else if(button == -2) {
        for (unsigned int i = 0; i < cp->points.size(); i++)
            if(cp->points[i]->isSelect && cp->points[i]->checkControlPointArea(x, y))
                cp->points[i]->dragSelectPoint(x, y);
    }
    if (cp->checkCollisionFirstPoint() && !raceOn)
        canApplyTransformations = true;
    if (canApplyTransformations && !raceOn) {
        if (translationMode)
            translate(x, y);
    } else if (translationMode && !canApplyTransformations) {
        translationMode = false;
        (*alert)->alerts.push_back(new Alert(950, 30, 200, 100, "Pista nao finalizada", Utils::WARNING, TRUE));
    }
}

/* Calcula os pontos da curva de Bezier de acordo com os pontos de controle
*/
void Bezier::calculatePointsBezier()
{
    int index = 0;
    for(float t = 0; t <= 1; t += 0.01) {
        float x = 0, y = 0;
        float lastP = cp->points.size() - 1;
        for(unsigned int i = 0; i < cp->points.size(); i++) {
            x += bernstein(lastP, i) * pow(1 - t, lastP - i) * pow(t, i) * cp->points[i]->point.x;
            y += bernstein(lastP, i) * pow(1 - t, lastP - i) * pow(t, i) * cp->points[i]->point.y;
        }
        estimatedPoints.at(index).x = x;
        estimatedPoints.at(index).y = y;
        index++;
    }
}

/* Desenha a curva de bezier baseada nos pontos estimados e calcula os demais pontos necessarios
   para o desenho da pista
*/
void Bezier::drawBezierCurve()
{
    int idx = 0;
    updateSpeedWaySize();
    for (float j = speedWayWidth; j > 0; j--) {
        for (unsigned int i = 1; i < estimatedPoints.size() - 1; i += 3) {
            Vector2 l = estimatedPoints[i - 1]; // left point
            Vector2 r = estimatedPoints[i + 1]; // center point
            Vector2 c = estimatedPoints[i]; // right point
            Vector2 n1 = (c - l).normalizeTo(j/2).getPerpendicular();
            Vector2 n2 = (r - c).normalizeTo(j/2).getPerpendicular();
            Vector2 ca = Utils::intersecLines2d(l + n1, c + n1, r + n2, c + n2); // pontos centrais
            Vector2 cb = Utils::intersecLines2d(l - n1, c - n1, r - n2, c - n2);
            bezierPointsOut.at(idx) = cb;
            bezierPointsIn.at(idx) = ca;
            idx++;
            rgb = Utils::RGBtoFloat(255,215,0);
            CV::color(rgb[0], rgb[1], rgb[2]);
            CV::line(estimatedPoints[i-1].x, estimatedPoints[i-1].y, estimatedPoints[i].x, estimatedPoints[i].y);
        }
    }
}

/* Desenha as linhas interna e externa da pista
*/
void Bezier::drawBezierCurveForPolygon()
{
    rgb = Utils::RGBtoFloat(105,105,105);
    CV::color(rgb[0], rgb[1], rgb[2]);
    unsigned int currentSize = (!canApplyTransformations) ? INDEX_POLY : bezierPointsIn.size();
    for (unsigned int i = 1; i < currentSize; i++) {
        CV::line(bezierPointsIn[i-1].x, bezierPointsIn[i-1].y, bezierPointsIn[i].x, bezierPointsIn[i].y);
        CV::line(bezierPointsOut[i-1].x, bezierPointsOut[i-1].y, bezierPointsOut[i].x, bezierPointsOut[i].y);
    }
}

/* Retorna a centroide da curva
   @return Vector2: coordenadas x, y do ponto central
*/
Vector2 Bezier::getCenterPoint()
{
    if (cp->points.size() <= 2)
        return (cp->points[0]->point + cp->points[1]->point) / 2;
    float cx = 0, cy = 0, det = 0, aux = 0;
    int j = 0;
    for (unsigned int i = 0; i < cp->points.size(); i++) {
        if (i + 1 == cp->points.size()) j = 0;
        else j = i + 1;
        Vector2 p = cp->points[i]->point;
        Vector2 p2 = cp->points[j]->point;
        aux = p.x * p2.y - p2.x * p.y;
        det += aux;
        cx += (p.x + p2.x) * aux;
        cy += (p.y + p2.y) * aux;
    }
    cx /= 3 * det;
    cy /= 3 * det;
    return Vector2{cx, cy};
}

/* Faz a reescala da pista a partir do centro
   @param n: 1 para ir aumentando a pista e -1 para ir diminuindo
*/
void Bezier::rescaleCurve(int n) {
    if (canApplyTransformations && !raceOn) {
        scale = (previousN != n) ? 0 : scale + n;
        previousN = n;
        int idx = 0;
        Vector2 l, r, c;
        for (unsigned int i = 0; i < cp->points.size() - 1; i++) {
            l = (i == 0) ? cp->points[cp->points.size() - 1]->point : cp->points[i - 1]->point;
            r = cp->points[i + 1]->point; // center point
            c = cp->points[i]->point; // right point
            Vector2 n1 = (c - l).normalizeTo(scale).getPerpendicular();
            Vector2 n2 = (r - c).normalizeTo(scale).getPerpendicular();
            Vector2 cb = Utils::intersecLines2d(l - n1, c - n1, r - n2, c - n2);
            cp->points[i]->point = cb;
            idx++;
        }
        cp->points[cp->points.size() - 1]->point = cp->points[0]->point - 2;
    } else {
        (*alert)->alerts.push_back(new Alert(950, 30, 200, 100, "Opcao indisponivel", Utils::WARNING, TRUE));
    }
    render();
}

/* Rotaciona a pista a partir do centro
   @param rad: angulo em radianos que deseja-se rotacionar a pista
*/
void Bezier::rotateCurve(float rad) {
    if (canApplyTransformations && !raceOn) {
        center = getCenterPoint();
        for (unsigned int i = 0; i < cp->points.size(); i++)
            cp->points[i]->point = Utils::rotatePoint(cp->points[i]->point, center, rad);
    } else {
        (*alert)->alerts.push_back(new Alert(950, 30, 200, 100, "Pista nao finalizada", Utils::WARNING, TRUE));
    }
}

/* translada a pista de acordo com o centro
   @param x: coordenada x do mouse
   @param y: coordenada y do mouse
*/
void Bezier::translate(int x, int y) {
    Vector2 c = Vector2 {(float)x, (float)y};
    center = getCenterPoint();
    Vector2 diff = c - center;
    for (unsigned int i = 0; i < cp->points.size(); i++)
        cp->points[i]->point += diff;
}

/* Metodo auxilar para calculo da bezier
   @param n: posicao no vetor do ultimo ponto de controle adicionado
   @param i: posicao no vetor do ponto de controle atual
*/
double Bezier::bernstein(float n, float i)
{
    return (factorial(n) / (factorial(i) * factorial(n - i)));
}

/* Metodo auxilar para calcular o fatorial de um numero
   @param n: inteiro que deseja-se saber o fatorial
*/
double Bezier::factorial(int n)
{
    return (n == 0) ? 1 : n * factorial(n - 1);
}

/* Aumenta o tamanho dos pontos de controle de acordo com a largura desejada da curva
*/
void Bezier::updateSpeedWaySize()
{
    float reserved = INDEX_POLY * speedWayWidth;
    bezierPointsIn.resize(reserved);
    bezierPointsOut.resize(reserved);
    for(unsigned int i = 0; i < reserved; i++) {
        bezierPointsIn.at(i) = Vector2 {0, 0};
        bezierPointsOut.at(i) = Vector2 {0, 0};
    }
}
