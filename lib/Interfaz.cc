#include "../include/Interfaz.h"

Interfaz::Interfaz()
: arbol_r(new Arbol_R){
    // Creamos la ventana
    window.create(sf::VideoMode(N, N), "R-TREE");
    if(!font.loadFromFile("../res/font/LemonMilk.otf")){
        cout<<"NO HUBO CARGA DE FUENTE"<<endl;
    }
}
// DEPRECADO
void Interfaz::setArbol(Arbol_R *a){
    arbol_r = a;
}

// DEPRECADO
Interfaz::Interfaz(int wc, int hc, int we)
    : width_canvas(wc), height_canvas(hc), width_ext(we), arbol_r(new Arbol_R)
{
    window.create(sf::VideoMode(width_canvas + width_ext, height_canvas), "R-TREE");
    if(!font.loadFromFile("../res/font/LemonMilk.otf")){
        cout<<"NO HUBO CARGA DE FUENTE"<<endl;
    }
}

// DEPRECADO
bool Interfaz::inside_canvas(pair<int, int> coordenada)
{
    return true;
    return coordenada.first > 0 && coordenada.second > 0 && coordenada.first < width_canvas && coordenada.second < height_canvas;
}


void Interfaz::ingresar_coordenada(sf::Event &event)
{
    cout << "INSERCION DE COORDENADA:" << endl;
    while (window.isOpen())
    {
        // Consultar eventos
        if (!window.pollEvent(event))
            continue;

        // Cerrar ventana
        if (event.type == sf::Event::Closed){
            window.close();
            return;
        }
        
        // Boton presionado, continuar
        if (event.type != sf::Event::MouseButtonPressed)
            continue;
        // Boton presionado, continuar
        if (event.mouseButton.button != sf::Mouse::Left)
            continue;

        // Agarrar la posicion del mouse, la que indicará la posición del nuevo punto
        sf::Vector2i localPosition = sf::Mouse::getPosition(window);
        // Calculamos la coordenada ya que la posición vertical incrementa a medida que desciende el puntero
        pair<int, int> coordenada = {localPosition.x, int(window.getSize().y) - localPosition.y};
        // Debugear
        printf("arbolito->insertar({{%d, %d}});\n", coordenada.first, coordenada.second);
        // Proceder a insertar la coordenada a la función
        arbol_r->insertar({{coordenada.first, coordenada.second}});
        return;
    }
}

void Interfaz::ingresar_poligono(sf::Event &event)
{
    // Vector que almacenara todos las coordenadas
    vector<Punto> coordenadas;
    // Mientras la ventana siga abierta
    while (window.isOpen())
    {
        // Consultar eventos
        if (!window.pollEvent(event))
            continue;

        // Si se cierra, cerrar ventana
        if (event.type == sf::Event::Closed){
            window.close();
            return;
        }

        // Si presionamos la tecla P (para termianr la inserción de los puntos del polígono)
        if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::P)){
            // Si la cantidad de coordenadas supera a 2, es un poligono, por lo que se inserta al arbol
            if (coordenadas.size() > 2)
                // se llama a la función de insertar
                arbol_r->insertar(coordenadas);
            return;
        }

        // Si clickeamos en una parte del lienzo
        if (event.mouseButton.button == sf::Mouse::Left)
        {
            // Obtenemos la posición relativa del mouse
            sf::Vector2i localPosition = sf::Mouse::getPosition(window);
            // Calculamos la coordenada en el plano cartesiano
            pair<int, int> coordenada = {localPosition.x, int(window.getSize().y) - localPosition.y};

            // Agregamos la coordenada al vector que representa el poligono
            coordenadas.push_back({coordenada.first, coordenada.second});
            // Impresion (DEBUG)
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
    // cout << "ELIMINACION" << endl;
    while (1)
    {
        // Mientras no hayan otros eventos, continuar
        if (!window.pollEvent(event))
            continue;

        // COntinuar, si el boton ha sido presionado
        if (event.type != sf::Event::MouseButtonPressed)
            continue;

        // Continuar si no se presiona el boton izquierdo
        if (event.mouseButton.button != sf::Mouse::Left)
            continue;

        // Obtenemos la posicion relativa del mouse en la ventana
        sf::Vector2i localPosition = sf::Mouse::getPosition(window);
        // Calcular la coordenada carteisana
        pair<int, int> coordenada = {localPosition.x, int(window.getSize().y) - localPosition.y};

        // Funcion DEBUG
        printf("arbolito->eliminar({%d, %d});\n", coordenada.first, coordenada.second);
        // Proceder a eliminar
        arbol_r->eliminar({coordenada.first, coordenada.second});

        // terminar función
        return;
    }
}

void Interfaz::eventos()
{
    sf::Event event;
    // COnsultar los enventos
    while (window.pollEvent(event))
    {
        // Si se cierra la vevntana, cerrar
        if (event.type == sf::Event::Closed)
            window.close();
        // La interfaz solo acepta teclas como entradas para las diferentes operaciones
        if (event.type != sf::Event::KeyPressed)
            continue;

        // Insercion de PUNTO
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
            this->ingresar_coordenada(event);
        // Insercion de  POLIGONO
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
            this->ingresar_poligono(event);
        // Eliminación del más cercano
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
            this->eliminar(event);
        // Busqueda de los K vecinos más cercanos
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::B))
            this->buscar_k_coordenadas(event);
    }
}

void Interfaz::buscar_k_coordenadas(sf::Event& event){
    // Variable que define la cantidad de vecinos a hallar
    int k {};
    // DEBUG
    cout<<"BUSQUEDA DE "<<k<<" VECINOS:"<<endl;
    // Mientras la ventana siga abierta
    while (window.isOpen())
    {
        // Continuar si no hay eventos
        if (!window.pollEvent(event))
            continue;

        // Si se llama al cierre de la ventana, cerrar la ventana
        if (event.type == sf::Event::Closed){
            window.close();
            return;
        }

        // Si se ingresa un texto, capturar cada caracter
        if(event.type == sf::Event::TextEntered){
            char numero = static_cast<char>(event.text.unicode);
            // Si el caracter es de un digito
            if(numero >= '0' && numero <= '9'){
                // Multiplicar por 10 el K actual
                k*=10;
                // Agregar el digito entrado a A
                k+=(numero-'0');
            }
            // DEBUG
            std::cout<<k<<std::endl;
        }
        
        // Si k== 0 continuar
        if(k<1) continue;

        // Mientras no se presiona el mouse, continuar
        if (event.type != sf::Event::MouseButtonPressed)
            continue;

        // Mientras no se presiona el mouse izquierda, continuar
        if (event.mouseButton.button != sf::Mouse::Left)
            continue;

        // Obtener la posición relativa del puntero para hallar los vecinos
        sf::Vector2i localPosition = sf::Mouse::getPosition(window);
        // Obtenemos la coordenada cartesiana
        pair<int, int> coordenada = {localPosition.x, int(window.getSize().y) - localPosition.y};
        // Definir la linea a dibujar para el vecino
        sf::Vertex linea[2]{ 
            sf::Vector2f(localPosition.x, localPosition.y)
        };
        linea[0].color = sf::Color::White;

        // DEBUG
        cout << coordenada.first << '\t' << coordenada.second << endl;
        // Llamada a la busqueda de vecinos cercanos por la función de busqueda del arbol
        vector<Arbol_R::Distante> k_vecinos = arbol_r->buscar_k_vecinos({coordenada.first, coordenada.second}, k);
        // Iterar por cada vecino en el vector
        for(auto i: k_vecinos){
            // Definir el punto medio del objeto vecino
            double pm_x=0, pm_y=0;
            // facilitar el acceso al objeto
            Entrada_Hoja* iEH = dynamic_cast<Entrada_Hoja*>(i.entrada);
            // Adicionar al punto medio los valores de cada punto
            for(auto p: iEH->tuplas){
                pm_x+=p.x;
                pm_y+=p.y;
            }
            // Dividir y obtener el centro por media
            pm_x/=iEH->tuplas.size();
            pm_y/=iEH->tuplas.size();
            
            // Definir las posiciones de la linea
            linea[1] = sf::Vector2f(static_cast<int>(pm_x), window.getSize().y - static_cast<int>(pm_y));
            linea[1].color = sf::Color::White;
            cout<<linea[1].position.x<<"  "<<linea[1].position.y<<endl;
            // Debigujar la linea de donde se hizo click a los diferentes vecinos cercanos
            window.draw(linea, 2, sf::Lines);
            window.display();
        }
        // Desdibujar las lineas a los vecinos cuando se vuelva a hacer click
        while(window.isOpen()){
            // Continuar si no hay eventos
            if (!window.pollEvent(event))
            continue;
            
            // Cerrar ventana
            if (event.type == sf::Event::Closed){
                window.close();
                return;
            }

            // Continaur si no se presiona el mouse
            if (event.type != sf::Event::MouseButtonPressed)
                continue;

            // Continaur si no se presiona el click izqs
            if (event.mouseButton.button != sf::Mouse::Left)
                continue;
            // Terminar funciony desdibujar lineas
            return;
        }
    }
}

// void Interfaz::marco(){
//     sf::RectangleShape rect(sf::Vector2f(width_canvas, height_canvas));
//     rect.setOutlineColor(sf::Color::Magenta);
//     rect.setOutlineThickness(2);
//     rect.setFillColor(sf::Color::Transparent);
//     window.draw(rect);
// }

void Interfaz::ejecutar()
{
    // Mientras la ventana este abierta
    while (window.isOpen())
    {   
        // Definir que funcion del arbol se llamará
        this->eventos();

        // Limpiar ventana
        window.clear();
        // Dibujar arbol 
        imprimir_arbol_r();
        // Mostrar dibujos
        window.display();
    }
}

void Interfaz::imprimir_arbol_r(){
    int espacio{10};
    // Dibujamos empezando en la raiz
    imprimir_arbol_r_recursivo(arbol_r->raiz, espacio, 30);
}

void Interfaz::imprimir_arbol_r_recursivo(Nodo* nodo, int &espacio, int color){
    // Si es un nodo hoja
    if(nodo->hoja){
        // por todas las entradas
        for(auto i: nodo->entradas){
            Entrada_Hoja* eh = dynamic_cast<Entrada_Hoja*>(i);
            // si es solo un punto
            if(eh->tuplas.size() == 1){
                // Dibujamos un circulo en la posición del Punto
                sf::CircleShape coordenada;
                coordenada.setRadius(4);
                coordenada.setFillColor(sf::Color::Red);
                coordenada.setPosition(eh->tuplas[0].x - 2, window.getSize().y - eh->tuplas[0].y - 2);
                window.draw(coordenada);

                // Imprimir coordendas
                /* sf::Text text;
                text.setFont(font);
                text.setString("Punto: "+tupla_string(eh->objeto[0]));
                text.setCharacterSize(10);
                text.setFillColor(sf::Color::White);
                text.setPosition(sf::Vector2f(window.getSize().x*7.f/10.f, window.getSize().y/10 + espacio));
                window.draw(text);
                espacio += 13; */
            }
            // Si es un poligono
            else
            {
                // Agregamos las coordenadas a la figura compleja
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

                /* // sdfsfs
                sf::Text text;
                text.setFont(font);
                text.setString("Poligono: "+polygon_coords);
                text.setCharacterSize(10);
                text.setFillColor(sf::Color::White);
                text.setPosition(sf::Vector2f(window.getSize().x*7.f/10.f, window.getSize().y/10 + espacio));
                window.draw(text);
                espacio += 13; */
            }
        }
    }
    // SI es un nodo interno
    else
    {
        // Pasamos por las entradas
        for (auto i : nodo->entradas)
        {
            // Obtenemos las distancias del MBR
            int dx = i->intervalos[0].i2 - i->intervalos[0].i1;
            int dy = i->intervalos[1].i2 - i->intervalos[1].i1;
            // Cremaos un rectangulo con las distancias
            sf::RectangleShape rectangulo(sf::Vector2f(dx, dy));
            rectangulo.setOutlineColor(sf::Color((40-color)%256, (2*color  + 10)%256, color%256));
            rectangulo.setOutlineThickness(2);
            // Posicionamos el rectangulo de acuerdo a su posición superior izquierda
            rectangulo.setPosition(i->intervalos[0].i1, window.getSize().y - i->intervalos[1].i2);
            rectangulo.setFillColor(sf::Color::Transparent);

            window.draw(rectangulo);
            // llamamos a la misma funión para al nodo hijo de la entrada interna
            imprimir_arbol_r_recursivo(dynamic_cast<Entrada_Interna*>(i)->puntero_hijo, espacio, color+30);
        }
    }
}

// FUNCION DEPRECADA
string Interfaz::tupla_string(Punto llave_tupla){
    return "("+to_string(llave_tupla.x)+", "+to_string(llave_tupla.y)+")";
}