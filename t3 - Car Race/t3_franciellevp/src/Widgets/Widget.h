//*************************************************************
//
// classe base para todos os widgets do programa
//
//*************************************************************
#ifndef __WIDGET_H__
#define __WIDGET_H__

#include <vector>

class Widget
{
    public:
        int x, y;
        float height, width;
        std::vector<float> bgColor;

        Widget() {}
        ~Widget() {}
        Widget(int x, int y, float w, float h, std::vector<float> bg)
        {
            this->x = x;
            this->y = y;
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
            if(mx >= x && mx <= (x + width) && my >= y && my <= (y + height))
                return true;
            return false;
        }
        /* Metodos que devem ser implementados pelas classes derivadas
        */
        virtual void renderWidgets() = 0;
        virtual void checkState (int state, int _x, int _y) = 0;
        virtual void render() = 0;
        virtual void create() = 0;
};

#endif

