#include "../include/R_MBR.h"

R_MBR::R_MBR(vector<pair<int, int>> tuplas)
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
void R_MBR::redimensionar_tuplas(vector<pair<int, int>> tuplas)
{
    extremos[0] = tuplas[0];
    extremos[1] = tuplas[0];
    for (auto i : tuplas)
    {
        if (i.first < extremos[0].first)
            extremos[0].first = i.first;
        if (i.first > extremos[1].first)
            extremos[1].first = i.first;
        if (i.second < extremos[1].second)
            extremos[1].second = i.second;
        if (i.second > extremos[0].second)
            extremos[0].second = i.second;
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
bool R_MBR::dentro(pair<int, int> llave_tupla)
{
    bool en_x = llave_tupla.first >= extremos[0].first && llave_tupla.first <= extremos[1].first;
    bool en_y = llave_tupla.second <= extremos[0].second && llave_tupla.second >= extremos[1].second;
    return en_x && en_y;
}