#include "../include/R_MBR.h"

R_MBR::R_MBR(vector<R_Info> tuplas)
{
    this->redimensionar_tuplas(tuplas);
}
R_MBR::R_MBR(vector<pair<R_MBR, R_Nodo *>> mbrs)
{
    this->redimensionar_mbrs(mbrs);
}

int R_MBR::agrandamiento(pair<int, int> tupla)
{
    pair<int, int> temp_extremos[2]{extremos[0], extremos[1]};
    // para x
    if (tupla.first < temp_extremos[0].first && tupla.first < temp_extremos[1].first)
        temp_extremos[0].first = tupla.first;
    else if (tupla.first > temp_extremos[0].first && tupla.first > temp_extremos[1].first)
        temp_extremos[1].first = tupla.first;
    // para y
    if (tupla.second < temp_extremos[0].second && tupla.second < temp_extremos[1].second)
        temp_extremos[1].second = tupla.second;
    else if (tupla.second > temp_extremos[0].second && tupla.second > temp_extremos[1].second)
        temp_extremos[0].second = tupla.second;

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
        if (!i.poligono)
        {
            extremos[0].first = min(extremos[0].first, i.info_tupla.first);
            extremos[1].first = max(extremos[1].first, i.info_tupla.first);
            extremos[1].second = min(extremos[1].second, i.info_tupla.second);
            extremos[0].second = max(extremos[0].second, i.info_tupla.second);
        }
        else{
            extremos[0].first = min(extremos[0].first, i.info_poligono.extremos[0].first);
            extremos[1].first = max(i.info_poligono.extremos[1].first, extremos[1].first);
            extremos[1].second = min(i.info_poligono.extremos[1].second, extremos[1].second);
            extremos[0].second = max(i.info_poligono.extremos[0].second, extremos[0].second);
        }
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
    if (!llave_tupla.poligono)
    {
        en_x = llave_tupla.info_tupla.first >= extremos[0].first && llave_tupla.info_tupla.first <= extremos[1].first;
        en_y = llave_tupla.info_tupla.second <= extremos[0].second && llave_tupla.info_tupla.second >= extremos[1].second;
    }
    else
    {
        en_x = llave_tupla.info_poligono.extremos[0].first >= extremos[0].first && llave_tupla.info_poligono.extremos[1].first <= extremos[1].first;
        en_y = llave_tupla.info_poligono.extremos[0].second <= extremos[0].second && llave_tupla.info_poligono.extremos[1].second >= extremos[1].second;
    }
    return en_x && en_y;
}