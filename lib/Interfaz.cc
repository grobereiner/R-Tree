#include "../include/Interfaz.h"

Interfaz::Interfaz(int wc, int hc, int we)
    : width_canvas(wc), height_canvas(hc), width_ext(we)
{
    window.create(sf::VideoMode(width_canvas + width_ext, height_canvas), "R-TREE");
}

bool Interfaz::inside_canvas(pair<int, int> coordenada){
    return coordenada.first > 0 && coordenada.second > 0 && coordenada.first < width_canvas && coordenada.second < height_canvas;
}

void Interfaz::ingresar_coordenada(){
    cout<<"INGRESAR COORDENADA A INSERTAR: ";
    pair<int, int> coordenada;
    cin >> coordenada.first >> coordenada.second;
    if (inside_canvas(coordenada)){
        arbolito.insercion(coordenada);
    }
}

void Interfaz::ingresar_poligono(){
    vector<pair<int,int>> coordenadas;
    cout<<"INGRESAR COORDENDAS DE POLIGONOS (x=0 o y=0 para terminar):"<<endl;
    while(1){
        pair<int,int> coordenada;
        cin >> coordenada.first >> coordenada.second;
        if(!inside_canvas(coordenada))
            break;
        coordenadas.push_back(coordenada);
    }
    if(coordenadas.size() > 2)
        arbolito.insercion(coordenadas);
}

void Interfaz::eliminar(){
    cout << "INGRESAR COORDENADA A ELIMINAR: ";
    pair<int, int> coordenada;
    cin >> coordenada.first >> coordenada.second;
    if (coordenada.first > 0 && coordenada.second > 0 && coordenada.first < width_canvas && coordenada.second < height_canvas)
    {
        // cout << "INTENTAR ELIMINAR: " << coordenada.first << '\t' << coordenada.second << endl;
        arbolito.eliminacion(coordenada);
    }
}

void Interfaz::eventos()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::MouseButtonPressed)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2i localPosition = sf::Mouse::getPosition(window);
                pair<int, int> coordenada = {localPosition.x, int(window.getSize().y) - localPosition.y};

                if (coordenada.first > 0 && coordenada.second > 0 && coordenada.first < width_canvas && coordenada.second < height_canvas)
                {
                    cout << "INSERCION: " << coordenada.first << '\t' << coordenada.second << endl;
                    arbolito.insercion(coordenada);
                }
            }
        }
        if(event.type == sf::Event::KeyPressed){
            // PUNTO
            // if(sf::Keyboard::isKeyPressed(sf::Keyboard::C)){
            //     this->ingresar_coordenada();
            // }
            // POLIGONO
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::P)){
                this->ingresar_poligono();
            }
            // ELIMINAR
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::E)){
                this->eliminar();
            }
        }
        if (event.type == sf::Event::Closed)
            window.close();
    }
}

void Interfaz::ejecutar()
{
    while (window.isOpen())
    {

        this->eventos();

        window.clear();
        arbolito.print_sfml(window);
        window.display();
    }
}