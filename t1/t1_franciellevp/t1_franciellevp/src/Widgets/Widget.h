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
        std::vector<Widget> wds;

        Widget();
        ~Widget();
        Widget(int x, int y, float w, float h, std::vector<float> bg);
        bool Colidiu(int mx, int my);
        virtual void RenderWidgets() = 0;
        virtual void CheckState (int state, int _x, int _y) = 0;
        virtual void Render() = 0;
        virtual void Create() = 0;
};

#endif

