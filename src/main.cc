#include "../include/R_Tree.h"
#include<SFML/Graphics.hpp>

int main()
{
    R_Tree arbolito;
    arbolito.insercion({114,371});
    arbolito.insercion({572,34});
    arbolito.insercion({253,10});
    arbolito.insercion({117,159});
    arbolito.insercion({128,33});
    arbolito.insercion({300,283});
    arbolito.insercion({502,166});
    arbolito.insercion({200,187});
    arbolito.insercion({343,593});
    arbolito.insercion({31,409});
    arbolito.insercion({578,408});
    arbolito.insercion({549,99});
    arbolito.insercion({488,132});
    arbolito.insercion({291,2});
    arbolito.insercion({469,271});
    arbolito.insercion({177,336});
    arbolito.insercion({43,501});
    arbolito.insercion({370,48});
    arbolito.insercion({511,487});
    arbolito.insercion({207,40});
    arbolito.insercion({272,507});
    arbolito.insercion({75,526});
    arbolito.insercion({73,276});
    arbolito.insercion({465,169});
    arbolito.insercion({21,497});
    arbolito.print_desmos();
    // arbolito.eliminacion({0,-3});
    // arbolito.eliminacion({-3,-3});
    // arbolito.eliminacion({3,-6});

    sf::RenderWindow window(sf::VideoMode(600, 600), "SFML works!");
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        arbolito.print_sfml(window);
        window.display();
    }



    return 0;
}