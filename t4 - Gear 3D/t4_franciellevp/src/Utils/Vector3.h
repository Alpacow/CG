#ifndef __VECTOR_3_H__
#define __VECTOR_3_H__

#include <math.h>

class Vector3
{
public:
    float x, y, z;

    Vector3()
    {
        x = y = z = 0.0;
    };

    Vector3(const float tx, const float ty, const float tz )
    {
        x = tx;
        y = ty;
        z = tz;
    };

    Vector3 (const Vector3& v)
    {
        this->x = v.x;
        this->y = v.y;
        this->z = v.z;
    };

    void setVector3(const float tx, const float ty, const float tz)
    {
        x = tx;
        y = ty;
        z = tz;
    };

    void setVector3(const Vector3 v)
    {
        x = v.x;
        y = v.y;
        z = v.z;
    };

    inline Vector3 operator + ( const Vector3 v )
    {
        Vector3 aux( x + v.x, y + v.y, z + v.z );
        return( aux );
    }

    inline Vector3 operator += (const Vector3& v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        Vector3 aux(x, y, z);
        return( aux );
    }

    inline Vector3 operator - ( const Vector3 v )
    {
        Vector3 aux( x - v.x, y - v.y, z - v.z );
        return( aux );
    }

    inline Vector3 operator * ( const float v )
    {
        Vector3 aux(x * v, y * v, z * v);
        return( aux );
    }

    inline Vector3  operator / ( const float s )
    {
        Vector3 aux( x / s, y / s, z / s );
        return( aux );
    }

    //	Produto escalar
    inline float operator * ( const Vector3 v )
    {
        float aux = x * v.x + y * v.y + z * v.z;
        return( aux );
    }

    //	Produto vetorial
    inline Vector3 operator ^ ( Vector3 v )
    {
        Vector3 aux( y * v.z - z * v.y,
                     z * v.x - x * v.z,
                     x * v.y - y * v.x );
        return( aux );
    }

    //  norma do vetor
    float norma()
    {
        return (float)(sqrt( x * x + y * y + z * z ));
    }

    float distance(Vector3 v)
    {
        return((*this - v).norma());
    }

    //	normaliza o vetor
    Vector3 normalize()
    {
        return( (*this) / (this->norma()) );
    }

    //	extende tamanho do vetor
    Vector3 extend (float val)
    {
        return ((*this) * (val) );
    }

};

#endif
