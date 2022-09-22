#include "../../include/Interfaz/Interfaz.h"

Interfaz::Interfaz(int wc, int hc, int we)
    : width_canvas(wc), height_canvas(hc), width_ext(we)
{
    window.create(sf::VideoMode(width_canvas + width_ext, height_canvas), "R-TREE");
}

bool Interfaz::inside_canvas(pair<int, int> coordenada)
{
    return coordenada.first > 0 && coordenada.second > 0 && coordenada.first < width_canvas && coordenada.second < height_canvas;
}

void Interfaz::ingresar_coordenada(sf::Event &event)
{
    cout << "INSERCION DE COORDENADA:" << endl;
    while (1)
    {
        if (!window.pollEvent(event))
            continue;
        if (event.type != sf::Event::MouseButtonPressed)
            continue;
        if (event.mouseButton.button != sf::Mouse::Left)
            continue;

        sf::Vector2i localPosition = sf::Mouse::getPosition(window);
        pair<int, int> coordenada = {localPosition.x, int(window.getSize().y) - localPosition.y};

        if (inside_canvas(coordenada))
        {
            cout << coordenada.first << '\t' << coordenada.second << endl;
            arbolito.insercion(coordenada);
        }
        return;
    }
}

void Interfaz::ingresar_poligono(sf::Event &event)
{
    cout << "INSERCION DE POLIGONO:" << endl;
    vector<pair<int, int>> coordenadas;
    while (1)
    {
        if (!window.pollEvent(event))
            continue;
        if (event.type != sf::Event::MouseButtonPressed)
            continue;

        if (event.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2i localPosition = sf::Mouse::getPosition(window);
            pair<int, int> coordenada = {localPosition.x, int(window.getSize().y) - localPosition.y};
            if (!inside_canvas(coordenada))
            {
                if (coordenadas.size() > 2)
                    arbolito.insercion(coordenadas);
                return;
            }

            coordenadas.push_back(coordenada);
            cout << coordenada.first << '\t' << coordenada.second << endl;

            // Visualizar como se esta creando el poligono
            sf::CircleShape coordenada_vista;
            coordenada_vista.setRadius(2);
            coordenada_vista.setFillColor(sf::Color::Yellow);
            coordenada_vista.setPosition(coordenada.first - 2, window.getSize().y - coordenada.second - 2);
            window.draw(coordenada_vista);
            window.display();
        }
    }
}

void Interfaz::eliminar(sf::Event &event)
{
    cout << "ELIMINACION" << endl;
    while (1)
    {
        if (!window.pollEvent(event))
            continue;
        if (event.type != sf::Event::KeyPressed)
            continue;

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
        {
            this->eliminar_coordenada(event);
            return;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
        {
            this->eliminar_poligono(event);
            return;
        }
    }
}

void Interfaz::eventos()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();
        if (event.type != sf::Event::KeyPressed)
            continue;

        // PUNTO
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
            this->ingresar_coordenada(event);
        // POLIGONO
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
            this->ingresar_poligono(event);
        // ELIMINAR
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
            this->eliminar(event);

    }
}

void Interfaz::marco(){
    sf::RectangleShape rect(sf::Vector2f(width_canvas, height_canvas));
    rect.setOutlineColor(sf::Color::Magenta);
    rect.setOutlineThickness(2);
    rect.setFillColor(sf::Color::Transparent);
    window.draw(rect);
}

void Interfaz::ejecutar()
{
    while (window.isOpen())
    {   
        this->eventos();

        window.clear();
        this->marco();
        arbolito.print_sfml(window);
        window.display();
    }
}

void Interfaz::eliminar_coordenada(sf::Event &event)
{
    cout << "INGRESAR COORDENADA A ELIMINAR: ";
    pair<int, int> coordenada;
    cin >> coordenada.first >> coordenada.second;
    if (inside_canvas(coordenada))
        arbolito.eliminacion(coordenada);
}

void Interfaz::eliminar_poligono(sf::Event &event)
{
    cout << "ELIMINACION DE POLIGONO" << endl;
    while (1)
    {
        if (!window.pollEvent(event))
            continue;

        if (event.type != sf::Event::MouseButtonPressed)
            continue;

        if (event.mouseButton.button != sf::Mouse::Left)
            continue;

        sf::Vector2i localPosition = sf::Mouse::getPosition(window);
        pair<int, int> coordenada = {localPosition.x, int(window.getSize().y) - localPosition.y};

        if (inside_canvas(coordenada))
        {
            cout << coordenada.first << '\t' << coordenada.second << endl;
            arbolito.eliminacion(coordenada);
        }
        return;
    }
}