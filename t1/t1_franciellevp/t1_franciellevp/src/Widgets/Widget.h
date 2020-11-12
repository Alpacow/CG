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
        // REVER SE DEVE FICAR AQ OU N
        // recebe as coordenadas do mouse para tratar clique ou detectar quando o mouse esta em cima do botao
        bool Colidiu(int mx, int my)
        {
            if(mx >= x && mx <= (x + width) && my >= y && my <= (y + height))
                return true;
            return false;
        }
        virtual void RenderWidgets() = 0;
        virtual void CheckState (int state, int _x, int _y) = 0;
        virtual void Render() = 0;
        virtual void Create() = 0;
};

#endif

