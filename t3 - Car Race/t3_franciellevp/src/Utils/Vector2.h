#ifndef __VECTOR_2_H__
#define __VECTOR_2_H__

#include <math.h>
#include <iostream>

class Vector2
{
public:
    float x, y;

    Vector2()
    {
        x = y = 0;
    }

    Vector2(float _x, float _y)
    {
        x = _x;
        y = _y;
    }

    void set(float _x, float _y)
    {
        x = _x;
        y = _y;
    }

    void normalize()
    {
        float norm = (float)sqrt(x*x + y*y);

        if(norm==0.0) {
            std::cout << "Normalize::Divisao por zero" << std::endl;
            x = 1;
            y = 1;
            return;
        }
        x /= norm;
        y /= norm;
    }

    Vector2 normalizeTo(float value)
    {
        normalize();
        x *= value;
        y *= value;
        Vector2 aux(x, y);
        return aux;
    }

    Vector2 scale (float xv, float yv)
    {
        Vector2 aux(x * xv, y * yv);
        return (aux);
    }

    Vector2 getPerpendicular()
    {
        Vector2 aux(-y, x);
        return( aux );
    }

    float angleBetween (const Vector2 v)
    {
        return acos((x * v.x + y * v.y) / (sqrt(x*x + y*y) * sqrt(v.x*v.x + v.y*v.y)));
    }

    Vector2 operator - (const Vector2& v)
    {
        Vector2 aux( x - v.x, y - v.y);
        return( aux );
    }

    Vector2 operator + (const Vector2& v)
    {
        Vector2 aux( x + v.x, y + v.y);
        return( aux );
    }

    Vector2 operator += (const Vector2& v)
    {
        x += v.x;
        y += v.y;
        Vector2 aux(x, y);
        return( aux );
    }

    Vector2 operator * (const Vector2& v)
    {
        Vector2 aux( x * v.x, y * v.y);
        return( aux );
    }

    Vector2 operator / (const Vector2& v)
    {
        if (v.x != 0 && v.y != 0) {
            Vector2 aux( x / v.x, y / v.y);
            return( aux );
        }
        return {x, y};
    }

    Vector2 operator = (const float v)
    {
        Vector2 aux(v, v);
        return( aux );
    }

    Vector2 operator / (const float v)
    {
        if (v != 0) {
            Vector2 aux( x / v, y / v);
            return( aux );
        }
        return {x, y};
    }

    Vector2 operator * (const float v)
    {
        Vector2 aux( x * v, y * v);
        return( aux );
    }

    Vector2 operator ^ (const float v)
    {
        Vector2 aux( pow(x, v), pow(y, v));
        return( aux );
    }

    Vector2 operator *= (const float v)
    {
        x *= v;
        y *= v;
        Vector2 aux(x, y);
        return( aux );
    }

    Vector2 operator += (const float v)
    {
        x += v;
        y += v;
        Vector2 aux(x, y);
        return( aux );
    }

    Vector2 operator -= (const float v)
    {
        x -= v;
        y -= v;
        Vector2 aux(x, y);
        return( aux );
    }
};

#endif
