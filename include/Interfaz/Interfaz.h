#ifndef INTEFRAZ_H
#define INTEFRAZ_H

#include <SFML/Graphics.hpp>
#include <utility>
#include <iostream>
#include "../R_Entidades/R_Tree.h"

using namespace std;

class Interfaz
{
public:
    Interfaz(int wc, int hc, int we);
    void ejecutar();
protected:
    void marco();
    void eventos();
    void ingresar_coordenada(sf::Event& event);
    void ingresar_poligono(sf::Event& event);
    void eliminar(sf::Event& event);
private:
    bool inside_canvas(pair<int, int> coordenada);
    void eliminar_coordenada(sf::Event& event);
    void eliminar_poligono(sf::Event& event);

    R_Tree arbolito;
    sf::RenderWindow window;
    int width_canvas;
    int height_canvas;
    int width_ext;
};

#endif