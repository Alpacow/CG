#ifndef CUBO_H_INCLUDED
#define CUBO_H_INCLUDED

#include <vector>
#include <iostream>

#define screenX 1600
#define screenY 900

class Cubo{
private:
    float x;
    float y;
    float z;
    float lado;
    std::vector<Objeto*> Pontos;
public:
    Cubo(float x, float y, float z, float lado);
    void MoveZ(float dist);
    void MoveY(float dist);
    void Draw3D(float dist_screen);
    void Draw2D(float dist_screen);

};

Cubo::Cubo(float x, float y, float z, float lado){
    this->x = x;
    this->y = y;
    this->z = z;
    this->lado = lado;

    float m = lado/2;
    //Parte para atras do Z
    this->Pontos.push_back(new Objeto(x-m, y-m, z+m));
    this->Pontos.push_back(new Objeto(x-m, y+m, z+m));
    this->Pontos.push_back(new Objeto(x+m, y+m, z+m));
    this->Pontos.push_back(new Objeto(x+m, y-m, z+m));

    //Parte para frente do Z
    this->Pontos.push_back(new Objeto(x-m, y-m, z-m));
    this->Pontos.push_back(new Objeto(x-m, y+m, z-m));
    this->Pontos.push_back(new Objeto(x+m, y+m, z-m));
    this->Pontos.push_back(new Objeto(x+m, y-m, z-m));

}

void Cubo::Draw3D(float dist_screen){

    Objeto* origem = new Objeto(800, 450, 0);
    float lado = 100;

    line(origem->x, origem->y, screenX, origem->y);

    std::vector<Objeto*> sample;

     sample.push_back(new Objeto(this->Pontos[0]->z + origem->x, this->Pontos[0]->y + origem->y, 0));
     sample.push_back(new Objeto(this->Pontos[1]->z + origem->x, this->Pontos[1]->y + origem->y, 0));
     sample.push_back(new Objeto(this->Pontos[5]->z + origem->x, this->Pontos[5]->y + origem->y, 0));
     sample.push_back(new Objeto(this->Pontos[4]->z + origem->x, this->Pontos[4]->y + origem->y, 0));

     for(auto i = 0; i<3; i++){
        line(sample[i]->x, sample[i]->y, sample[i+1]->x, sample[i+1]->y);
     }
    line(sample[0]->x, sample[0]->y, sample[3]->x, sample[3]->y);

    rectFill(origem->x + dist_screen, 0, origem->x + dist_screen + 10, 2000);

    line(origem->x, origem->y, origem->x + dist_screen, screenY);
    line(origem->x, origem->y, origem->x + dist_screen, 0);
}

void Cubo::MoveZ(float dist){

    for(auto i = 0; i<8; i++){
        this->Pontos[i]->z += dist;
    }

}

void Cubo::MoveY(float dist){

    for(auto i = 0; i<8; i++){
        this->Pontos[i]->y += dist;
    }

}

void Cubo::Draw2D(float dist_screen){

    Objeto* origem = new Objeto(400, 450, 0);

    std::vector<Objeto*> Pontos2d;
    for(auto i = 0; i<8; i++){
            Pontos2d.push_back(new Objeto(this->Pontos[i]->x*dist_screen/this->Pontos[i]->z, this->Pontos[i]->y*dist_screen/this->Pontos[i]->z, 0));
    }
    for(auto i = 0; i<8; i++){
        Pontos2d[i]->x += origem->x;
        Pontos2d[i]->y += origem->y;

    }

    line(Pontos2d[0]->x, Pontos2d[0]->y, Pontos2d[1]->x, Pontos2d[1]->y);
    line(Pontos2d[1]->x, Pontos2d[1]->y, Pontos2d[2]->x, Pontos2d[2]->y);
    line(Pontos2d[2]->x, Pontos2d[2]->y, Pontos2d[3]->x, Pontos2d[3]->y);
    line(Pontos2d[0]->x, Pontos2d[0]->y, Pontos2d[3]->x, Pontos2d[3]->y);

    line(Pontos2d[4]->x, Pontos2d[4]->y, Pontos2d[5]->x, Pontos2d[5]->y);
    line(Pontos2d[5]->x, Pontos2d[5]->y, Pontos2d[6]->x, Pontos2d[6]->y);
    line(Pontos2d[6]->x, Pontos2d[6]->y, Pontos2d[7]->x, Pontos2d[7]->y);
    line(Pontos2d[4]->x, Pontos2d[4]->y, Pontos2d[7]->x, Pontos2d[7]->y);

    line(Pontos2d[1]->x, Pontos2d[1]->y, Pontos2d[5]->x, Pontos2d[5]->y);
    line(Pontos2d[2]->x, Pontos2d[2]->y, Pontos2d[6]->x, Pontos2d[6]->y);
    line(Pontos2d[3]->x, Pontos2d[3]->y, Pontos2d[7]->x, Pontos2d[7]->y);
    line(Pontos2d[4]->x, Pontos2d[4]->y, Pontos2d[0]->x, Pontos2d[0]->y);

}

#endif // CUBO_H_INCLUDED
