//****************************************************************
//
// classe derivada de Widget que controla os inputs da aplicacao
//
//****************************************************************
#include "Input.h"
#include "../Canvas/gl_canvas2d.h"
#include "../Utils/Utils.h"

#define MAX_CHARS 162

using namespace std;

/* Inicia todos os atributos necessarios
   @param img: instancia da classe Bmp para controlar as acoes dos botoes na imagem
*/
Input::Input () : Widget()
{
    create();
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
    CV::text(p.x, p.y - 10, label.c_str()); //escreve o label do botao mais ou menos ao centro.

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
void Input::checkState(int state, int x, int y)
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
        cout << inputs[i]->buffer << endl;
        if(inputs[i]->canWrite) { // TODO: add suporte para tipos: int/ float (acc apenas numeros do teclado)
            if (key == 8)
                inputs[i]->buffer.pop_back();
            else
                inputs[i]->buffer += (char)key;
        }
    }
}

/* Cria todos os botoes da aplicacao
*/
void Input::create()
{
    vector<float> bg = Utils::RGBtoFloat(28,28,28);
    inputs.push_back(new Input(1030, 400, 130, 30, bg, "Largura da pista", bg));
}

bool Input::getCanWrite()
{
    return canWrite;
}
