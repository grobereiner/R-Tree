#include"../include/Entrada_Hoja.h"

Entrada_Hoja::~Entrada_Hoja() = default;

Entrada_Hoja::Entrada_Hoja() = default;

Entrada_Hoja::Entrada_Hoja(const vector<Punto> &t): tuplas(t){

    // Actualizar los valores del intervalo 
    intervalos[0] = {numeric_limits<int>::max(), numeric_limits<int>::min()};
    intervalos[1] = {numeric_limits<int>::max(), numeric_limits<int>::min()};

    for(int i = 0; i<tuplas.size(); i++){
        intervalos[0].i1 = min(intervalos[0].i1, tuplas[i].x);
        intervalos[1].i1 = min(intervalos[1].i1, tuplas[i].y);
        intervalos[0].i2 = max(intervalos[0].i2, tuplas[i].x);
        intervalos[1].i2 = max(intervalos[1].i2, tuplas[i].y);
    }
}

// FUNCION DEPRECADA
bool Entrada_Hoja::dentro(Punto P){
    Punto extremo{numeric_limits<int>::max() / 2, P.y};
    int disminuir{};
    int conteo_intersecciones{};
    int i{};
    do
    {
        int siguiente = (i + 1) % tuplas.size();
        if (tuplas[i].y == P.y)
            disminuir++;
        if (intersecta(tuplas[i], tuplas[siguiente], P, extremo))
        {
            if (orientacion(tuplas[i], P, tuplas[siguiente]) == colineal)
                return enSegmento(tuplas[i], P, tuplas[siguiente]);
            conteo_intersecciones++;
        }
        i = siguiente;
    } while (i != 0);

    conteo_intersecciones -= disminuir;
    if (conteo_intersecciones % 2)
        return true;
    return false;
}

bool Entrada_Hoja::enSegmento(Punto a, Punto x, Punto b){
    if (x.x <= max(a.x, b.x) && x.x >= min(a.x, b.x))
        if (x.y <= max(a.y, b.y) && x.y >= min(a.y, b.y))
            return true;
    return false;
}

// FUNCION DEPRECADA
Entrada_Hoja::orientacion_t Entrada_Hoja::orientacion(Punto a, Punto x, Punto b){
    int valor = (x.y - a.y) * (b.y - x.y) - (x.x - a.x) * (b.x - x.x);
    if (!valor)
        return colineal;
    if (valor > 0)
        return horario;
    return antihorario;
}

// FUNCION DEPRECADA
bool Entrada_Hoja::intersecta(Punto a1, Punto a2, Punto b1, Punto b2){
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