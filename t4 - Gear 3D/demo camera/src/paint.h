#ifndef PAINT_H_INCLUDED
#define PAINT_H_INCLUDED

#include <vector>


class Paint{
private:
    bool active;
    std::vector<Objeto*> points;
public:
    Paint();
    void Draw();
    void Erase();
    void Toggle();
    void AddPoint(int x, int y);
};

Paint::Paint(){
    this->active = false;
}

void Paint::AddPoint(int x, int y){
    if(this->active){
        this->points.push_back(new Objeto(x, y, 0));
    }
}

void Paint::Draw(){
    color(0,0,0);
    if(this->points.size()> 1){
        for(int i = 1; i < this->points.size(); i++){
            if(this->points[i]->x != -1 && this->points[i-1]->x != -1)
            line(this->points[i]->x, this->points[i]->y, this->points[i-1]->x, this->points[i-1]->y);
        }
    }
}


void Paint::Toggle(){
    if(this->active){
        this->AddPoint(-1,-1);
        this->active = false;
    }
    else
        this->active = true;
}

void Paint::Erase(){
    this->points.clear();
}



#endif // PAINT_H_INCLUDED
