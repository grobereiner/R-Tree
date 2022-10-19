#include "../include/Interfaz.h"

Interfaz::Interfaz(int wc, int hc, int we)
    : width_canvas(wc), height_canvas(hc), width_ext(we), arbol_r(new Arbol_R)
{
    window.create(sf::VideoMode(width_canvas + width_ext, height_canvas), "R-TREE");
    if(!font.loadFromFile("../res/font/LemonMilk.otf")){
        cout<<"NO HUBO CARGA DE FUENTE"<<endl;
    }
}

bool Interfaz::inside_canvas(pair<int, int> coordenada)
{
    return coordenada.first > 0 && coordenada.second > 0 && coordenada.first < width_canvas && coordenada.second < height_canvas;
}

void Interfaz::ingresar_coordenada(sf::Event &event)
{
    cout << "INSERCION DE COORDENADA:" << endl;
    while (window.isOpen())
    {
        if (!window.pollEvent(event))
            continue;

        if (event.type == sf::Event::Closed){
            window.close();
            return;
        }
        
        if (event.type != sf::Event::MouseButtonPressed)
            continue;
        if (event.mouseButton.button != sf::Mouse::Left)
            continue;

        sf::Vector2i localPosition = sf::Mouse::getPosition(window);
        pair<int, int> coordenada = {localPosition.x, int(window.getSize().y) - localPosition.y};

        if (inside_canvas(coordenada))
        {
            cout << coordenada.first << '\t' << coordenada.second << endl;
            arbol_r->insertar({{coordenada.first, coordenada.second}});
        }
        return;
    }
}

void Interfaz::ingresar_poligono(sf::Event &event)
{
    cout << "INSERCION DE POLIGONO:" << endl;
    vector<Punto> coordenadas;
    while (window.isOpen())
    {
        if (!window.pollEvent(event))
            continue;

        if (event.type == sf::Event::Closed){
            window.close();
            return;
        }
        
        // if (event.type != sf::Event::MouseButtonPressed)
        //     continue;

        if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::P)){
            if (coordenadas.size() > 2)
                arbol_r->insertar(coordenadas);
            return;
        }

        if (event.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2i localPosition = sf::Mouse::getPosition(window);
            pair<int, int> coordenada = {localPosition.x, int(window.getSize().y) - localPosition.y};
            if (!inside_canvas(coordenada) )
            {
                if (coordenadas.size() > 2)
                    arbol_r->insertar(coordenadas);
                return;
            }

            coordenadas.push_back({coordenada.first, coordenada.second});
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

        if (event.type != sf::Event::MouseButtonPressed)
            continue;

        if (event.mouseButton.button != sf::Mouse::Left)
            continue;

        sf::Vector2i localPosition = sf::Mouse::getPosition(window);
        pair<int, int> coordenada = {localPosition.x, int(window.getSize().y) - localPosition.y};

        if (inside_canvas(coordenada))
        {
            cout << coordenada.first << '\t' << coordenada.second << endl;
            arbol_r->eliminar_cercano({coordenada.first, coordenada.second});
        }
        return;
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
        // BUSCAR K VECINOS COORD
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::B))
            this->buscar_k_coordenadas(event);
    }
}

void Interfaz::buscar_k_coordenadas(sf::Event& event){
    int k {};
    cout<<"BUSQUEDA DE "<<k<<" VECINOS:"<<endl;
    while (window.isOpen())
    {
        if (!window.pollEvent(event))
            continue;

        if (event.type == sf::Event::Closed){
            window.close();
            return;
        }

        if(event.type == sf::Event::TextEntered){
            char numero = static_cast<char>(event.text.unicode);
            if(numero >= '0' && numero <= '9'){
                k*=10;
                k+=(numero-'0');
            }
            std::cout<<k<<std::endl;
        }
        
        if(k<1) continue;

        if (event.type != sf::Event::MouseButtonPressed)
            continue;

        if (event.mouseButton.button != sf::Mouse::Left)
            continue;

        sf::Vector2i localPosition = sf::Mouse::getPosition(window);
        pair<int, int> coordenada = {localPosition.x, int(window.getSize().y) - localPosition.y};
        sf::Vertex linea[2]{ 
            sf::Vector2f(localPosition.x, localPosition.y)
        };
        linea[0].color = sf::Color::Green;

        if (inside_canvas(coordenada))
        {
            cout << coordenada.first << '\t' << coordenada.second << endl;
            vector<Arbol_R::Entrada_Distancia> k_vecinos = arbol_r->buscar_k_vecinos({coordenada.first, coordenada.second}, k);
            for(auto i: k_vecinos){
                double pm_x=0, pm_y=0;
                Entrada_Hoja* iEH = dynamic_cast<Entrada_Hoja*>(i.entrada);
                for(auto p: iEH->tuplas){
                    pm_x+=p.x;
                    pm_y+=p.y;
                }
                pm_x/=iEH->tuplas.size();
                pm_y/=iEH->tuplas.size();
                
                linea[1] = sf::Vector2f(static_cast<int>(pm_x), window.getSize().y - static_cast<int>(pm_y));
                linea[1].color = sf::Color::Green;
                cout<<linea[1].position.x<<"  "<<linea[1].position.y<<endl;
                window.draw(linea, 2, sf::Lines);
                window.display();
            }
        }
        while(window.isOpen()){
            if (!window.pollEvent(event))
            continue;
            
            if (event.type == sf::Event::Closed){
                window.close();
                return;
            }

            if (event.type != sf::Event::MouseButtonPressed)
                continue;

            if (event.mouseButton.button != sf::Mouse::Left)
                continue;
            return;
        }
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
        imprimir_arbol_r();
        window.display();
    }
}

void Interfaz::imprimir_arbol_r(){
    int espacio{10};
    imprimir_arbol_r_recursivo(arbol_r->raiz, espacio, 30);
}

void Interfaz::imprimir_arbol_r_recursivo(Nodo* nodo, int &espacio, int color){
    if(nodo->hoja){
        for(auto i: nodo->entradas){
            Entrada_Hoja* eh = dynamic_cast<Entrada_Hoja*>(i);
            if(eh->tuplas.size() == 1){
                sf::CircleShape coordenada;
                coordenada.setRadius(4);
                coordenada.setFillColor(sf::Color::Red);
                coordenada.setPosition(eh->tuplas[0].x - 2, window.getSize().y - eh->tuplas[0].y - 2);
                window.draw(coordenada);

                // Imprimir coordendas
                sf::Text text;
                text.setFont(font);
                text.setString("Punto: "+tupla_string(eh->tuplas[0]));
                text.setCharacterSize(10);
                text.setFillColor(sf::Color::White);
                text.setPosition(sf::Vector2f(window.getSize().x*7.f/10.f, window.getSize().y/10 + espacio));
                window.draw(text);
                espacio += 13;
            }
            else
            {
                sf::ConvexShape convex;
                string polygon_coords;
                convex.setPointCount(eh->tuplas.size());
                for (int j = 0; j < eh->tuplas.size(); j++)
                {
                    convex.setPoint(j, sf::Vector2f(eh->tuplas[j].x, window.getSize().y - eh->tuplas[j].y));
                    polygon_coords = polygon_coords + tupla_string(eh->tuplas[j]) + '\t';
                }
                convex.setFillColor(sf::Color::Blue);
                window.draw(convex);

                // sdfsfs
                sf::Text text;
                text.setFont(font);
                text.setString("Poligono: "+polygon_coords);
                text.setCharacterSize(10);
                text.setFillColor(sf::Color::White);
                text.setPosition(sf::Vector2f(window.getSize().x*7.f/10.f, window.getSize().y/10 + espacio));
                window.draw(text);
                espacio += 13;
            }
        }
    }
    else
    {
        for (auto i : nodo->entradas)
        {
            int dx = i->intervalos[0].i2 - i->intervalos[0].i1;
            int dy = i->intervalos[1].i2 - i->intervalos[1].i1;
            sf::RectangleShape rectangulo(sf::Vector2f(dx, dy));
            rectangulo.setOutlineColor(sf::Color((40-color)%256, (2*color  + 10)%256, color%256));
            rectangulo.setOutlineThickness(2);
            rectangulo.setPosition(i->intervalos[0].i1, window.getSize().y - i->intervalos[1].i2);
            rectangulo.setFillColor(sf::Color::Transparent);

            window.draw(rectangulo);
            imprimir_arbol_r_recursivo(dynamic_cast<Entrada_Interna*>(i)->puntero_hijo, espacio, color+30);
        }
    }
}

string Interfaz::tupla_string(Punto llave_tupla){
    return "("+to_string(llave_tupla.x)+", "+to_string(llave_tupla.y)+")";
}