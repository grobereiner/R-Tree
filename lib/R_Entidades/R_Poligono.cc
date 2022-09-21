#include "../../include/R_Entidades/R_Poligono.h"

R_Poligono::R_Poligono(vector<pair<int, int>> p)
    : tuplas(p)
{
    extremos[0] = p[0];
    extremos[1] = p[0];
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

bool R_Poligono::enSegmento(pair<int, int> a, pair<int, int> x, pair<int, int> b)
{
    if (x.first <= max(a.first, b.first) && x.first >= min(a.first, b.first))
        if (x.second <= max(a.second, b.second) && x.second >= min(a.second, b.second))
            return true;
    return false;
}

R_Poligono::orientacion_t R_Poligono::orientacion(pair<int, int> a, pair<int, int> x, pair<int, int> b)
{
    int valor = (x.second - a.second) * (b.second - x.second) - (x.first - a.first) * (b.first - x.first);
    if (!valor)
        return colineal;
    if (valor > 0)
        return horario;
    return antihorario;
}

// a1a2
// b1b2
bool R_Poligono::intersecta(pair<int, int> a1, pair<int, int> a2, pair<int, int> b1, pair<int, int> b2)
{
    orientacion_t or1 = orientacion(a1, a2, b1);
    orientacion_t or2 = orientacion(a1, a2, b2);
    orientacion_t or3 = orientacion(b1, b2, a1);
    orientacion_t or4 = orientacion(b1, b2, a2);

    if (or1 != or2 && or3 != or4)
        return true;

    if (or1 == colineal && enSegmento(a1, b1, a2))
        return true;
    if (or2 == colineal && enSegmento(a1, b2, a2))
        return true;
    if (or3 == colineal && enSegmento(b1, a1, b2))
        return true;
    if (or4 == colineal && enSegmento(b1, a2, b2))
        return true;

    return false;
}

bool R_Poligono::dentro(pair<int, int> tupla)
{
    pair<int, int> extremo{numeric_limits<int>::max() / 2, tupla.second};
    int disminuir{};
    int conteo_intersecciones{};
    int i{};
    do
    {
        int siguiente = (i + 1) % tuplas.size();
        if (tuplas[i].second == tupla.second)
            disminuir++;
        if (intersecta(tuplas[i], tuplas[siguiente], tupla, extremo))
        {
            if (orientacion(tuplas[i], tupla, tuplas[siguiente]) == colineal)
                return enSegmento(tuplas[i], tupla, tuplas[siguiente]);
            conteo_intersecciones++;
        }
        i = siguiente;
    } while (i != 0);

    conteo_intersecciones -= disminuir;
    if (conteo_intersecciones % 2)
        return true;
    return false;
}