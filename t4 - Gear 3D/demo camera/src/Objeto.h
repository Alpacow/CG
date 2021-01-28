#ifndef OBJETO_H_INCLUDED
#define OBJETO_H_INCLUDED

//Classe mãe das outras, e também serve como uma Classe Ponto

class Objeto{
public:
   Objeto(float x, float y, float z);
   void Set_pos(float x, float y, float z);
   float x;
   float y;
   float z;
};

Objeto::Objeto(float x, float y, float z){
    this->x = x;
    this->y = y;
    this->z = z;
}

void Objeto::Set_pos(float x, float y, float z){
   this->x = x;
   this->y = y;
   this->z = z;
}

#endif // OBJETO_H_INCLUDED
