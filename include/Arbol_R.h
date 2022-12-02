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
    // Insertar 
    void insertar(vector<Punto> Ps);
    // Eliminacion deprecada
    void eliminar(Punto P);
    // Eliminar 
    void eliminar_cercano(Punto P);
    // Buscar los KNN
    vector<Distante> buscar_k_vecinos(Punto P, int k);
    // Objeto KNN
    struct Distante{
        Distante(Entrada* E, Punto P, Nodo* N);
        Entrada* entrada;
        double distancia;
        Nodo* tupla;
    };
    // BUSQUEDA EXACTA
    bool buscar_exacto(const vector<Punto>& Ps);

    // OBTENER SOBRELAPADO TOTAL
    double obtener_sobrelapado_total();
    // RETORNAR ALTURA DEL ARBOL
    int retornar_altura();
    // Obtener solapamiento de un nivel
    double obtener_sobrelapado(int Nivel);
protected:
    // COMPARAR OBEJTOS KNN
    friend bool operator<(const Distante &pd1, const Distante &pd2);
    friend bool operator>(const Distante &pd1, const Distante &pd2);
    friend bool operator<=(const Distante &pd1, const Distante &pd2);
    friend bool operator>=(const Distante &pd1, const Distante &pd2);

    // Escoger la hoja apropiada para insercion
    Nodo* escoger_hoja(Entrada_Hoja *E);
    // Partir un nodo
    Nodo* partir_nodo(Entrada *E, Nodo *L);
    // Ajustar intervalos  y agregar nuevos nodos en nodos superiores
    Nodo* ajustar_arbol(Nodo *L , Nodo* LL);

    // Ubicar la hoja donde el objeto se encuentra
    Nodo* hallar_hoja(Nodo* E, Punto P);
    // Condensación DEPRECADA
    void condensar(Nodo* &H, deque<Nodo*> &NE);
    // Actualizar nodos superiores en eliminación
    void condensar_cercano(Nodo* L);
    // Reinsertar nodos eliminados
    void insercion_recursiva(Nodo* N);
private:
    // Destruir memoria dinamica
    void destruir_recursivo(Nodo *N);
    // Comparar entradas por eje x
    static bool comparar_x(Entrada* a, Entrada* b);
    // Comparar entradas por eje y
    static bool comparar_y(Entrada* a, Entrada* b);
    // Nodo raiz
    Nodo* raiz;
    // N de objetos
    int objetos;

    friend void nodo_entradas(Arbol_R* a);
    friend class Interfaz;
};

#endif