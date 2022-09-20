#ifndef INTEFRAZ_H
#define INTEFRAZ_H

#include <SFML/Graphics.hpp>
#include <utility>
#include <iostream>
#include "../include/R_Tree.h"

using namespace std;

class Interfaz
{
public:
    Interfaz(int wc, int hc, int we);
    void ejecutar();
protected:
    void eventos();
    void ingresar_coordenada();
    void ingresar_poligono();
    void eliminar();
private:
    bool inside_canvas(pair<int, int> coordenada);

    R_Tree arbolito;
    sf::RenderWindow window;
    int width_canvas;
    int height_canvas;
    int width_ext;
};

#endif