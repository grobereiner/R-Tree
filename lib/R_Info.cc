#include "../include/R_Info.h"

R_Info::R_Info(pair<int, int> c)
    : info_tupla(c), poligono(false) {}

R_Info::R_Info(vector<pair<int, int>> p)
    : poligono(true), info_poligono(p){}