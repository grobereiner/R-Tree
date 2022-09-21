#ifndef R_TREE_H
#define R_TREE_H

#include <iostream>
#include <deque>
#include <algorithm>
#include <string>
#include <cmath>
#include "R_Nodo.h"
#include "R_MBR.h"
#include <SFML/Graphics.hpp>

/**
 * @brief Clase que indexa información bidimensional en una estructura R-Tree
 *
 */
class R_Tree
{
public:
    /**
     * @brief Construye un objeto R-Tree
     *
     */
    R_Tree();
    /**
     * @brief Funcion que imprime el objeto R-Tree en una ventana SFML
     *
     * @param ventana Referencia a la ventana creada en SFML mediante sf::RenderWindow
     */
    void print_sfml(sf::RenderWindow &ventana);
    /**
     * @brief Funcion que inserta una coordenada o poligono en la estructura
     *
     * @param llave_tupla Se ingresa una coordenada como pair<int, int> o un poligono como vector<pair<int,int>>
     */
    void insercion(R_Info llave_tupla);
    /**
     * @brief Funcion que elimina una coordenada o poligono en la estructura
     *
     * @param llave_tupla Se ingresa una coordenada, esta elimina una coordenada si existe coincidencia o un poligono si se encuentra adentro
     */
    void eliminacion(R_Info llave_tupla);

protected:
    void condensar(R_Nodo *&nodo, deque<R_Nodo *> &nodos_eliminados);

    void insertar_entradas(R_Nodo *nodo);

    R_Nodo *hallar_hoja(R_Nodo *nodo, R_Info llave_tupla);
    void print_sfml_recursiva(R_Nodo *nodo, sf::RenderWindow &ventana, int &espacio);
    static bool comparar_x_tupla(R_Info a, R_Info b);
    static bool comparar_x_mbr(pair<R_MBR, R_Nodo *> a, pair<R_MBR, R_Nodo *> b);
    static bool comparar_y_tupla(R_Info a, R_Info b);
    static bool comparar_y_mbr(pair<R_MBR, R_Nodo *> a, pair<R_MBR, R_Nodo *> b);
    static string tupla_string(pair<int, int> llave_tupla);

    R_Nodo *partir_nodo_mbrs(R_Nodo *&nodo, R_Nodo *otro_nodo_interno);

    R_Nodo *partir_nodo_tuplas(R_Nodo *&nodo, R_Info llave_tupla);

    R_Nodo *escoger_nodo(R_Nodo *nodo, R_Info llave_tupla);

    void ajustar_arbol(R_Nodo *&objetivo, R_Nodo *nuevo_nodo);

private:
    R_Nodo *root;
    sf::Font font;
};

#endif