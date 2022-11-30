#ifndef ARBOL_R_H
#define ARBOL_R_H

#include <iostream>
#include<vector>
#include<algorithm>
#include<limits>
#include<deque>
#include<queue>
#include<cmath>
#include<functional>
#include<stack>
#include<set>

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
    struct Distante;
    Arbol_R();
    ~Arbol_R();
    void insertar(vector<Punto> Ps);
    void eliminar(Punto P);
    void eliminar_cercano(Punto P);
    vector<Distante> buscar_k_vecinos(Punto P, int k);
    struct Distante{
        Distante(Entrada* E, Punto P, Nodo* N);
        Entrada* entrada;
        double distancia;
        Nodo* tupla;
    };
    bool buscar_exacto(const vector<Punto>& Ps);

    double obtener_sobrelapado_total();
    int retornar_altura();
    double obtener_sobrelapado(int Nivel);
protected:
    friend bool operator<(const Distante &pd1, const Distante &pd2);
    friend bool operator>(const Distante &pd1, const Distante &pd2);
    friend bool operator<=(const Distante &pd1, const Distante &pd2);
    friend bool operator>=(const Distante &pd1, const Distante &pd2);

    Nodo* escoger_hoja(Entrada_Hoja *E);
    Nodo* partir_nodo(Entrada *E, Nodo *L);
    Nodo* ajustar_arbol(Nodo *L , Nodo* LL);
    Nodo* hallar_hoja(Nodo* E, Punto P);
    void condensar(Nodo* &H, deque<Nodo*> &NE);
    void condensar_cercano(Nodo* L);
    void insercion_recursiva(Nodo* N);
private:
    void destruir_recursivo(Nodo *N);
    static bool comparar_x(Entrada* a, Entrada* b);
    static bool comparar_y(Entrada* a, Entrada* b);
    Nodo* raiz;
    int objetos;

    friend void nodo_entradas(Arbol_R* a);
    friend class Interfaz;
};

#endif