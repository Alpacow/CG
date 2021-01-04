//****************************************************************
//
// classe derivada de Widget que controla os inputs da aplicacao
//
//****************************************************************
#include "Input.h"
#include "../Canvas/gl_canvas2d.h"
#include "../Utils/Utils.h"
#include <string>

#define MAX_CHARS 162

using namespace std;

/* Inicia todos os atributos necessarios
   @param img: instancia da classe Bmp para controlar as acoes dos botoes na imagem
*/
Input::Input (Car** car) : Widget()
{
    this->carController = car;
    create();
    vector<int> rgb = Utils::FloatToRGB((*car)->carColor[0], (*car)->carColor[1], (*car)->carColor[2]);
    inputs[0]->buffer = to_string(rgb[0]);
    inputs[1]->buffer = to_string(rgb[1]);
    inputs[2]->buffer = to_string(rgb[2]);
}

Input::~Input() {}

/* Inicia todos os atributos necessarios
   @param x: coordenada x do canto superior esquerdo que o alert comeca a ser desenhado
   @param y: coordenada y do canto superior esquerdo que o alert comeca a ser desenhado
   @param w: largura da janela
   @param h: altura da janela
   @param bg: array contendo a cor RGB do fundo do botao
   @param label: texto contido dentro do botao
   @param labelColor: array contendo a cor RGB do texto do botao
*/
Input::Input(int x, int y, float w, float h, vector<float> bg, const string label, vector<float> labelColor)
    : Widget(), label(label), labelColor(labelColor)
{
    this->p.x = x;
    this->p.y = y;
    this->width = w;
    this->height  = h;
    this->bgColor = bg;
    this->canWrite = false;
    this->buffer = "";
}

/* Renderiza/desenha o botao na tela
*/
void Input::render()
{
    if (canWrite)
        CV::color(1, 0, 0);
    else
        CV::color(bgColor[0], bgColor[1], bgColor[2]); // cor da volta
    CV::rectFill(p.x, p.y, p.x + width, p.y + height);
    CV::color(1, 1, 1); // cor de fundo sempre clara
    CV::rectFill(p.x + 2, p.y + 2, (p.x + width) - 2, (p.y + height) - 2);

    CV::color(labelColor[0], labelColor[1], labelColor[2]);
    CV::text(p.x, p.y - 10, label.c_str());

    CV::color(0, 0, 0);
    CV::text(p.x + 2, p.y + height/2, buffer.c_str());
}

/* Percorre um array contendo todos os botoes para desenha-los na tela
*/
void Input::renderWidgets()
{
    for(vector<Input>::size_type i = 0; i != inputs.size(); i++)
        inputs[i]->render();
}

/* Verifica a colisao com cada botao criado, realizando as operacoes necessarias quando colide
   @param state: verifica se botao do mouse foi clicado
   @param x: coordenada x do mouse
   @param y: coordenada y do mouse
*/
void Input::checkState(int button, int state, int x, int y)
{
    for(vector<Input>::size_type i = 0; i != inputs.size(); i++) {
        if( state == 0 ) {
            if(inputs[i]->checkCollision(x, y))
                inputs[i]->canWrite = true;
            else inputs[i]->canWrite = false;
        }
    }
}

void Input::keyboardCheck(int key)
{
    for(vector<Input>::size_type i = 0; i != inputs.size(); i++) {
        if(inputs[i]->canWrite) {
            if (key == 8)
                inputs[i]->buffer.pop_back();
            if (key >= 48 && key <= 57) { //TODO: deixar padrao de acordo com o tipo especificado
                if (inputs[i]->buffer.size() <= 1 && inputs[i]->buffer.compare(to_string(0)) == 0) {
                    inputs[i]->buffer.pop_back();
                    inputs[i]->buffer.push_back((char)key);
                } else
                    inputs[i]->buffer.push_back((char)key);
            }
        }
        if (inputs[i]->buffer.size() <= 0)
            inputs[i]->buffer = to_string(0);
    }
    (*carController)->carColor = Utils::RGBtoFloat(stoi(inputs[0]->buffer), stoi(inputs[1]->buffer), stoi(inputs[2]->buffer));
}

/* Cria todos os botoes da aplicacao
*/
void Input::create()
{
    vector<float> bg = Utils::RGBtoFloat(28,28,28);
    inputs.push_back(new Input(1030, 220, 40, 25, bg, "R", bg));
    inputs.push_back(new Input(1075, 220, 40, 25, bg, "G", bg));
    inputs.push_back(new Input(1120, 220, 40, 25, bg, "B", bg));
}

bool Input::getCanWrite()
{
    return canWrite;
}
