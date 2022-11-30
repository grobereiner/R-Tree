#include "../include/Entrada.h"

Entrada::Entrada(): intervalos(2){}
Entrada::~Entrada() = default;
bool Entrada::dentro(int x, int y){
    bool dentro_x = x>=intervalos[0].i1 && x<=intervalos[0].i2;
    bool dentro_y = y>=intervalos[1].i1 && y<=intervalos[1].i2;
    return dentro_x && dentro_y;
}