#ifndef INTEFRAZ_H
#define INTEFRAZ_H

#include <SFML/Graphics.hpp>
#include <utility>
#include <iostream>
#include "../R_Entidades/R_Tree.h"

using namespace std;

/**
 * @brief Contiene los objetos SFML y la estructura R-Tree para representar una interfaz gráfica
 * 
 */
class Interfaz
{
public:
    /**
     * @brief Construye un objeto Interfaz
     * 
     * @param wc La distancia horizontal del plano cartesiano
     * @param hc La distancia vertical del plano cartesiano
     * @param we La distancia horizontal donde se presentará la información
     */
    Interfaz(int wc, int hc, int we);
    /**
     * @brief Ejecuta el renderizado del objeto sf::RenderWindow
     * 
     */
    void ejecutar();

protected:
    void marco();
    void eventos();
    void ingresar_coordenada(sf::Event& event);
    void ingresar_poligono(sf::Event& event);
    void eliminar(sf::Event& event);

private:
    bool inside_canvas(pair<int, int> coordenada);

    R_Tree arbolito;
    sf::RenderWindow window;
    int width_canvas;
    int height_canvas;
    int width_ext;
};

#endif