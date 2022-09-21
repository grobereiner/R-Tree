#ifndef R_INFO_H
#define R_INFO_H

#include "R_Poligono.h"
#include<vector>
#include<utility>

using namespace std;

/**
 * @brief Clase que almacena la información bidimensiona: coordenada o poligono
 * 
 */
struct R_Info
{
    /**
     * @brief Construye un objeto R-info
     * 
     * @param c par que representa una tupla
     */
    R_Info(pair<int, int> c);
    /**
     * @brief Construye un objeto R-info
     * 
     * @param p vector de pares que representa un poligono
     */
    R_Info(vector<pair<int, int>> p);

    /**
     * @brief Sobrecarga de igualdad
     * 
     */
    bool operator==(const R_Info &other);
    /**
     * @brief Sobracarga de desigualdad
     * 
     */
    bool operator!=(const R_Info &other);
    /**
     * @brief Obtener el extremo izquierdo de una coordenada o poligono
     * 
     */
    int get_left();
    /**
     * @brief Obtener el extremo derecho de una coordenada o poligono
     * 
     */
    int get_right();
    /**
     * @brief Obtener el extremo superior de una coordenada o poligono
     * 
     */
    int get_up();
    /**
     * @brief Obtener el extremo inferior de una coordenada o poligono
     * 
     */
    int get_down();

    /**
     * @brief Par que representa la tupla
     * 
     */
    pair<int, int> info_tupla;
    /**
     * @brief Objeto R-Poligono que representa un poligono
     * 
     */
    R_Poligono info_poligono;
    /**
     * @brief booleano que indica si el objeto almacena una coordenada o un poligono
     * 
     */
    bool poligono;
};

#endif