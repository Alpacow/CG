//*************************************************************
//
// classe base para todos os widgets do programa
//
//*************************************************************
#ifndef __WIDGET_H__
#define __WIDGET_H__

#include "../Utils/Vector2.h"
#include "../Utils/Utils.h"
#include <vector>

class Widget
{
    public:
        Vector2 p;
        float height, width;
        std::vector<float> bgColor;

        Widget() {}
        ~Widget() {}
        Widget(int x, int y, float w, float h, std::vector<float> bg)
        {
            this->p.x = x;
            this->p.y = y;
            this->width = w;
            this->height  = h;
            this->bgColor = bg;
        }

        Widget(Vector2 p, float w, float h, std::vector<float> bg)
        {
            this->p = p;
            this->width = w;
            this->height  = h;
            this->bgColor = bg;
        }

        /* Detecta se o ouse esta em cima do widget
           @param mx: coordenada x do mouse
           @param my: coordenada y do mouse
        */
        bool checkCollision(int mx, int my)
        {
            return Utils::checkRectCollision(mx, my, p, Vector2{p.x + width, p.y + height});
        }
        /* Metodos que devem ser implementados pelas classes derivadas
        */
        virtual void renderWidgets() = 0;
        virtual void checkState (int button, int state, int x, int y) = 0;
        virtual void render() = 0;
        virtual void create() = 0;
        virtual void keyboardCheck(int key) = 0;
};

#endif

