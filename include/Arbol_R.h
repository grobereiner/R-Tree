#ifndef ARBOL_R_H
#define ARBOL_R_H

#include <iostream>
#include<vector>
#include<algorithm>
#include<limits>
#include<deque>
#include<queue>
#include<cmath>
#include<stack>

#include"Nodo.h"
#include"Entrada_Hoja.h"
#include"Entrada_Interna.h"
#include"Entrada.h"
#include"Constante.h"
#include"Punto.h"
#include"Interfaz.h"

using namespace std;

struct Interfaz;

class Arbol_R{
public:
    struct Entrada_Distancia;
    Arbol_R();
    void insertar(vector<Punto> Ps);
    void eliminar(Punto P);
    void eliminar_cercano(Punto P);
    vector<Entrada_Distancia> buscar_k_vecinos(Punto P, int k);
    struct Entrada_Distancia{
        Entrada_Distancia(Entrada* E, Punto P, Nodo* N);
        Entrada* entrada;
        double distancia;
        Nodo* tupla;
    };
protected:
    friend bool operator<(const Entrada_Distancia &pd1, const Entrada_Distancia &pd2);
    friend bool operator>(const Entrada_Distancia &pd1, const Entrada_Distancia &pd2);
    friend bool operator<=(const Entrada_Distancia &pd1, const Entrada_Distancia &pd2);
    friend bool operator>=(const Entrada_Distancia &pd1, const Entrada_Distancia &pd2);

    Nodo* escoger_hoja(Entrada_Hoja *E);
    Nodo* partir_nodo(Entrada *E, Nodo *L);
    Nodo* ajustar_arbol(Nodo *L , Nodo* LL);
    Nodo* hallar_hoja(Nodo* E, Punto P);
    void condensar(Nodo* &H, deque<Nodo*> &NE);
    void condensar_cercano(Nodo* L);
    void insercion_recursiva(Nodo* N);
private:
    static bool comparar_x(Entrada* a, Entrada* b);
    static bool comparar_y(Entrada* a, Entrada* b);
    Nodo* raiz;

    friend class Interfaz;
};

#endif