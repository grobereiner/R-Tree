#ifndef INTEFRAZ_H
#define INTEFRAZ_H

#include <SFML/Graphics.hpp>
#include <utility>
#include <iostream>
#include "Arbol_R.h"

using namespace std;

class Arbol_R;

/**
 * @brief Contiene los objetos SFML y la estructura R-Tree para representar una interfaz gráfica
 * 
 */
class Interfaz
{
public:
    /**
     * @brief // Constructor para debuguear
     * 
     * @param wc La distancia horizontal del plano cartesiano
     * @param hc La distancia vertical del plano cartesiano
     * @param we La distancia horizontal donde se presentará la información
     */
    Interfaz(int wc, int hc, int we);
    
    // Constructor por defecto, aplicacion
    Interfaz();
    // Agregar arbol
    void setArbol(Arbol_R *a);
    // Ejecutar la ventana
    void ejecutar();

protected:
    //FUNCION DEPRECADA 
    void marco();
    // Definir que evento se solicita (teclas) para definir un metodo del arbol
    void eventos();
    // Metodo que utiliza la inserción de acuerdo al click del puntero 
    void ingresar_coordenada(sf::Event& event);
    // Metodo que utiliza la inserción de acuerdo multiples click del puntero 
    void ingresar_poligono(sf::Event& event);
    // Metodo que utiliza la busqeuda para hallar los vecinos de acuerdo a la posición del puntero
    void buscar_k_coordenadas(sf::Event& event);
    // Metodo que utiliza la eliminación para eliminar la entrada hoja mas cercana  a la posición del puntero
    void eliminar(sf::Event& event);

private:
    //FUNCION DEPRECADA 
    bool inside_canvas(pair<int, int> coordenada);
    // llamada a la función para dibujar el arbol
    void imprimir_arbol_r();
    // Llamada para cada nodo y dibujar el MBR , punto o poligono
    void imprimir_arbol_r_recursivo(Nodo* nodo, int &espacio, int color);
    //FUNCION DEPRECADA 
    string tupla_string(Punto llave_tupla);

    // Arbol
    Arbol_R *arbol_r;
    // Ventana
    sf::RenderWindow window;
    // Fuente de puntos (DEPRECADA)
    sf::Font font;

    // ATRIBUTOS DEPRECADOS
    int width_canvas;
    int height_canvas;
    int width_ext;
};

#endif