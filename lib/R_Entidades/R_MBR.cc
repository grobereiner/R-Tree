#include "../../include/R_Entidades/R_MBR.h"

R_MBR::R_MBR(R_Info tupla){
    extremos[0] = {tupla.info_tupla.first-6, tupla.info_tupla.second+6};
    extremos[1] = {tupla.info_tupla.first+6, tupla.info_tupla.second-6};
}

R_MBR::R_MBR(vector<R_Info> tuplas)
{
    this->redimensionar_tuplas(tuplas);
}
R_MBR::R_MBR(vector<pair<R_MBR, R_Nodo *>> mbrs)
{
    this->redimensionar_mbrs(mbrs);
}

int R_MBR::agrandamiento(R_Info tupla)
{
    pair<int, int> temp_extremos[2]{extremos[0], extremos[1]};
    // para x
    if (tupla.get_left() < temp_extremos[0].first && tupla.get_left() < temp_extremos[1].first)
        temp_extremos[0].first = tupla.get_left();
    else if (tupla.get_right() > temp_extremos[0].first && tupla.get_right() > temp_extremos[1].first)
        temp_extremos[1].first = tupla.get_right();
    // para y
    if (tupla.get_down() < temp_extremos[0].second && tupla.get_down() < temp_extremos[1].second)
        temp_extremos[1].second = tupla.get_down();
    else if (tupla.get_up() > temp_extremos[0].second && tupla.get_up() > temp_extremos[1].second)
        temp_extremos[0].second = tupla.get_up();

    int area = abs((extremos[0].first - extremos[1].first) * (extremos[0].second - extremos[1].second));
    int nueva_area = abs((temp_extremos[0].first - temp_extremos[1].first) * (temp_extremos[0].second - temp_extremos[1].second));
    return nueva_area - area;
}
void R_MBR::redimensionar_tuplas(vector<R_Info> tuplas)
{
    if (tuplas[0].poligono)
    {
        extremos[0] = tuplas[0].info_poligono.extremos[0];
        extremos[1] = tuplas[0].info_poligono.extremos[1];
    }
    else
    {
        extremos[0] = tuplas[0].info_tupla;
        extremos[1] = tuplas[0].info_tupla;
    }
    for (auto i : tuplas)
    {
        extremos[0].first = min(extremos[0].first, i.get_left());
        extremos[1].first = max(extremos[1].first, i.get_right());
        extremos[1].second = min(extremos[1].second, i.get_down());
        extremos[0].second = max(extremos[0].second, i.get_up());
    }
}

void R_MBR::redimensionar_mbrs(vector<pair<R_MBR, R_Nodo *>> mbrs)
{
    extremos[0] = mbrs[0].first.extremos[0];
    extremos[1] = mbrs[0].first.extremos[1];
    for (pair<R_MBR, R_Nodo *> i : mbrs)
    {
        if (i.first.extremos[0].first < extremos[0].first)
            extremos[0].first = i.first.extremos[0].first;
        if (i.first.extremos[1].first > extremos[1].first)
            extremos[1].first = i.first.extremos[1].first;
        if (i.first.extremos[1].second < extremos[1].second)
            extremos[1].second = i.first.extremos[1].second;
        if (i.first.extremos[0].second > extremos[0].second)
            extremos[0].second = i.first.extremos[0].second;
    }
}
bool R_MBR::dentro(R_Info llave_tupla)
{
    bool en_x, en_y;
    en_x = llave_tupla.get_left() >= extremos[0].first && llave_tupla.get_right() <= extremos[1].first;
    en_y = llave_tupla.get_up() <= extremos[0].second && llave_tupla.get_down() >= extremos[1].second;
    return en_x && en_y;
}