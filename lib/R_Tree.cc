#include "../include/R_Tree.h"

R_Tree::R_Tree() : root(new R_Nodo(true)) { root->padre = nullptr; }

void R_Tree::print_desmos()
{
    int tupla_id = 0;
    print_desmos(root, tupla_id);
    cout << '\n';
}

void R_Tree::print_sfml(sf::RenderWindow &ventana)
{
    print_sfml(root, ventana);
}

void R_Tree::insercion(R_Info llave_tupla)
{
    // I1
    R_Nodo *objetivo = escoger_nodo(root, llave_tupla);
    R_Nodo *nuevo_nodo{nullptr};
    // I2
    if (objetivo->llaves_tupla.size() < M)
        objetivo->llaves_tupla.push_back(llave_tupla);
    else
        nuevo_nodo = partir_nodo_tuplas(objetivo, llave_tupla);
    // I3
    ajustar_arbol(objetivo, nuevo_nodo);
}

void R_Tree::eliminacion(R_Info llave_tupla)
{
    // D1 encontrar la hoja que lo contiene
    R_Nodo *obj = hallar_hoja(root, llave_tupla);
    if (obj == nullptr)
        return;

    // D2 remover la coordenada
    for (int i = 0; i < obj->llaves_tupla.size(); i++)
    {
        if (obj->llaves_tupla[i] != llave_tupla)
            continue;
        obj->llaves_tupla.erase(next(obj->llaves_tupla.begin(), i));
        break;
    }

    // D3 Condensar arbol
    deque<R_Nodo *> nodos_eliminados;
    condensar(obj, nodos_eliminados);

    // D4 verificar si raiz tiene solo un hijo
    if (!root->hoja && root->llaves_MBR_hijo.size() == 1)
    {
        root = root->llaves_MBR_hijo[0].second;
        root->padre = nullptr;
    }
}

void R_Tree::condensar(R_Nodo *&nodo, deque<R_Nodo *> &nodos_eliminados)
{
    // CT2 verificar que no sea raiz
    if (nodo != root)
    {
        // CT3 eliminar el nodo si tiene menos de m entradas
        if (nodo->llaves_tupla.size() < m && nodo->llaves_MBR_hijo.size() < m)
        {

            nodos_eliminados.push_front(nodo);
            for (int i = 0; i < nodo->padre->llaves_MBR_hijo.size(); i++)
            {
                if (nodo->padre->llaves_MBR_hijo[i].second != nodo)
                    continue;
                nodo->padre->llaves_MBR_hijo.erase(next(nodo->padre->llaves_MBR_hijo.begin(), i));
                break;
            }
        }
        else
        {
            // CT4 reajustar los extremos del padre del nodo eliminado
            for (int i = 0; i < nodo->padre->llaves_MBR_hijo.size(); i++)
            {
                if (nodo->padre->llaves_MBR_hijo[i].second == nodo)
                {
                    if (nodo->hoja)
                        nodo->padre->llaves_MBR_hijo[i].first.redimensionar_tuplas(nodo->llaves_tupla);
                    else
                        nodo->padre->llaves_MBR_hijo[i].first.redimensionar_mbrs(nodo->llaves_MBR_hijo);
                    break;
                }
            }
        }
        condensar(nodo->padre, nodos_eliminados);
    }
    else
    {
        for (int i = 0; i < nodos_eliminados.size(); i++)
        {
            insertar_entradas(nodos_eliminados[i]);
        }
    }
}

void R_Tree::insertar_entradas(R_Nodo *nodo)
{
    if (nodo->hoja)
    {
        for (auto i : nodo->llaves_tupla)
        {
            insercion(i);
        }
    }
    else
    {
        for (auto i : nodo->llaves_MBR_hijo)
        {
            ajustar_arbol(i.second->padre, nodo);
            // insertar_entradas(i.second);
        }
    }
}

R_Nodo *R_Tree::hallar_hoja(R_Nodo *nodo, R_Info llave_tupla)
{
    if (nodo->hoja)
    {
        for (auto tupla : nodo->llaves_tupla)
        {
            if (llave_tupla == tupla)
            {
                return nodo;
            }
        }
        return nullptr;
    }
    for (auto mbr_hijo : nodo->llaves_MBR_hijo)
    {
        if (!mbr_hijo.first.dentro(llave_tupla))
            continue;
        R_Nodo *hallar = hallar_hoja(mbr_hijo.second, llave_tupla);
        if (hallar)
            return hallar;
    }
    return nullptr;
}

void R_Tree::print_desmos(R_Nodo *nodo, int &tupla_id)
{

    if (nodo->hoja)
    {
        for (auto i : nodo->llaves_tupla)
            printf("P_{%d} = (%d, %d)\n", tupla_id++, i.info_tupla.first, i.info_tupla.second);
    }
    else
    {
        for (auto i : nodo->llaves_MBR_hijo)
        {
            printf("\\operatorname{polygon}((%d, %d), (%d, %d), (%d, %d), (%d, %d))\n", i.first.extremos[0].first, i.first.extremos[0].second, i.first.extremos[1].first, i.first.extremos[0].second, i.first.extremos[1].first, i.first.extremos[1].second, i.first.extremos[0].first, i.first.extremos[1].second);
            print_desmos(i.second, tupla_id);
        }
    }
}

void R_Tree::print_sfml(R_Nodo *nodo, sf::RenderWindow &ventana)
{
    if (nodo->hoja)
    {
        for (auto i : nodo->llaves_tupla)
        {
            if (!i.poligono)
            {
                sf::CircleShape coordenada;
                coordenada.setRadius(4);
                coordenada.setFillColor(sf::Color::Red);
                coordenada.setPosition(i.info_tupla.first - 2, ventana.getSize().y - i.info_tupla.second - 2);
                ventana.draw(coordenada);
            }
            else
            {
                sf::ConvexShape convex;
                convex.setPointCount(i.info_poligono.tuplas.size());
                for (int j = 0; j < i.info_poligono.tuplas.size(); j++)
                {
                    convex.setPoint(j, sf::Vector2f(i.info_poligono.tuplas[j].first, ventana.getSize().y - i.info_poligono.tuplas[j].second));
                }
                convex.setFillColor(sf::Color::Blue);
                ventana.draw(convex);
            }
        }
    }
    else
    {
        for (auto i : nodo->llaves_MBR_hijo)
        {
            int dx = i.first.extremos[1].first - i.first.extremos[0].first;
            int dy = i.first.extremos[0].second - i.first.extremos[1].second;
            sf::RectangleShape rectangulo(sf::Vector2f(dx, dy));
            rectangulo.setOutlineColor(sf::Color::White);
            rectangulo.setOutlineThickness(2);
            rectangulo.setPosition(i.first.extremos[0].first, ventana.getSize().y - i.first.extremos[0].second);
            rectangulo.setFillColor(sf::Color::Transparent);

            ventana.draw(rectangulo);
            print_sfml(i.second, ventana);
        }
    }
}

bool R_Tree::comparar_x_tupla(R_Info a, R_Info b) { return a.get_left() < b.get_left(); }

bool R_Tree::comparar_x_mbr(pair<R_MBR, R_Nodo *> a, pair<R_MBR, R_Nodo *> b)
{
    if (a.first.extremos[0].first < b.first.extremos[0].first)
        return true;
    if (a.first.extremos[0].first == b.first.extremos[0].first)
        return a.first.extremos[1].first < b.first.extremos[1].first;
    return false;
}

bool R_Tree::comparar_y_tupla(R_Info a, R_Info b) { return a.get_down() < b.get_down(); }

bool R_Tree::comparar_y_mbr(pair<R_MBR, R_Nodo *> a, pair<R_MBR, R_Nodo *> b)
{
    if (a.first.extremos[1].second < b.first.extremos[1].second)
        return true;
    if (a.first.extremos[1].second == b.first.extremos[1].second)
        return a.first.extremos[0].second < b.first.extremos[1].second;
    return false;
}

R_Nodo *R_Tree::partir_nodo_mbrs(R_Nodo *&nodo, R_Nodo *otro_nodo_interno)
{
    int cota_inferior = ceil(0.4 * m), cota_superior = M - ceil(0.4 * m);
    vector<pair<R_MBR, R_Nodo *>> almacen{nodo->llaves_MBR_hijo};

    if (otro_nodo_interno->hoja)
        almacen.emplace_back(pair<R_MBR, R_Nodo *>{R_MBR(otro_nodo_interno->llaves_tupla), otro_nodo_interno});
    else
        almacen.emplace_back(pair<R_MBR, R_Nodo *>{R_MBR(otro_nodo_interno->llaves_MBR_hijo), otro_nodo_interno});

    vector<pair<R_MBR, R_Nodo *>> S1, S2;
    int perimetro_minimo = numeric_limits<int>::max();

    // Para X
    sort(almacen.begin(), almacen.end(), comparar_x_mbr);
    for (int i = cota_inferior; i <= cota_superior; i++)
    {
        vector<pair<R_MBR, R_Nodo *>> tS1(almacen.begin(), next(almacen.begin(), i)), tS2(next(almacen.begin(), i), almacen.end());
        int perimetro1 = 2 * abs(tS1[0].first.extremos[0].first - tS1[tS1.size() - 1].first.extremos[1].first) + 2 * abs(tS1[0].first.extremos[1].second - tS1[tS1.size() - 1].first.extremos[0].second);
        int perimetro2 = 2 * abs(tS2[0].first.extremos[0].first - tS2[tS2.size() - 1].first.extremos[1].first) + 2 * abs(tS2[0].first.extremos[1].second - tS2[tS2.size() - 1].first.extremos[0].second);
        if (perimetro1 + perimetro2 < perimetro_minimo)
        {
            perimetro_minimo = perimetro1 + perimetro2;
            S1 = tS1;
            S2 = tS2;
        }
    }

    // Para Y
    sort(almacen.begin(), almacen.end(), comparar_y_mbr);
    for (int i = cota_inferior; i <= cota_superior; i++)
    {
        vector<pair<R_MBR, R_Nodo *>> tS1(almacen.begin(), next(almacen.begin(), i)), tS2(next(almacen.begin(), i), almacen.end());
        int perimetro1 = 2 * abs(tS1[0].first.extremos[0].first - tS1[tS1.size() - 1].first.extremos[1].first) + 2 * abs(tS1[0].first.extremos[1].second - tS1[tS1.size() - 1].first.extremos[0].second);
        int perimetro2 = 2 * abs(tS2[0].first.extremos[0].first - tS2[tS2.size() - 1].first.extremos[1].first) + 2 * abs(tS2[0].first.extremos[1].second - tS2[tS2.size() - 1].first.extremos[0].second);
        if (perimetro1 + perimetro2 < perimetro_minimo)
        {
            perimetro_minimo = perimetro1 + perimetro2;
            S1 = tS1;
            S2 = tS2;
        }
    }

    nodo->llaves_MBR_hijo = S1;
    R_Nodo *nuevo_nodo = new R_Nodo(nodo->hoja);
    nuevo_nodo->llaves_MBR_hijo = S2;
    return nuevo_nodo;
}

R_Nodo *R_Tree::partir_nodo_tuplas(R_Nodo *&nodo, R_Info llave_tupla)
{
    int cota_inferior = m, cota_superior = M - m;
    vector<R_Info> almacen{nodo->llaves_tupla};
    almacen.push_back(llave_tupla);

    vector<R_Info> S1, S2;
    int perimetro_minimo = numeric_limits<int>::max();
    // Para X
    sort(almacen.begin(), almacen.end(), comparar_x_tupla);
    for (int i = cota_inferior; i <= cota_superior; i++)
    {
        vector<R_Info> tS1(almacen.begin(), next(almacen.begin(), i)), tS2(next(almacen.begin(), i), almacen.end());
        int perimetro1 = 2 * abs(tS1[0].get_left() - tS1[tS1.size() - 1].get_right()) + 2 * abs(tS1[0].get_down() - tS1[tS1.size() - 1].get_up());
        int perimetro2 = 2 * abs(tS2[0].get_left() - tS2[tS2.size() - 1].get_right()) + 2 * abs(tS2[0].get_down() - tS2[tS2.size() - 1].get_up());
        if (perimetro1 + perimetro2 < perimetro_minimo)
        {
            perimetro_minimo = perimetro1 + perimetro2;
            S1 = tS1;
            S2 = tS2;
        }
    }
    // Para Y
    sort(almacen.begin(), almacen.end(), comparar_y_tupla);
    for (int i = cota_inferior; i <= cota_superior; i++)
    {
        vector<R_Info> tS1(almacen.begin(), next(almacen.begin(), i)), tS2(next(almacen.begin(), i), almacen.end());
        int perimetro1 = 2 * abs(tS1[0].get_left() - tS1[tS1.size() - 1].get_right()) + 2 * abs(tS1[0].get_down() - tS1[tS1.size() - 1].get_up());
        int perimetro2 = 2 * abs(tS2[0].get_left() - tS2[tS2.size() - 1].get_right()) + 2 * abs(tS2[0].get_down() - tS2[tS2.size() - 1].get_up());
        if (perimetro1 + perimetro2 < perimetro_minimo)
        {
            perimetro_minimo = perimetro1 + perimetro2;
            S1 = tS1;
            S2 = tS2;
        }
    }

    nodo->llaves_tupla = S1;
    R_Nodo *nuevo_nodo = new R_Nodo(nodo->hoja);
    nuevo_nodo->llaves_tupla = S2;
    return nuevo_nodo;
}

R_Nodo *R_Tree::escoger_nodo(R_Nodo *nodo, R_Info llave_tupla)
{
    if (nodo->hoja)
        return nodo;

    int indice_optimo;
    int incremento_area_minimo{numeric_limits<int>::max()};
    for (int i = 0; i < nodo->llaves_MBR_hijo.size(); i++)
    {
        int incremento_relativo{nodo->llaves_MBR_hijo[i].first.agrandamiento(llave_tupla)};
        if (incremento_relativo < incremento_area_minimo)
        {
            indice_optimo = i;
            incremento_area_minimo = incremento_relativo;
        }
    }
    return escoger_nodo(nodo->llaves_MBR_hijo[indice_optimo].second, llave_tupla);
}

void R_Tree::ajustar_arbol(R_Nodo *&objetivo, R_Nodo *nuevo_nodo)
{
    if (nuevo_nodo == nullptr)
    {
        if (objetivo->padre != nullptr)
        {
            int indice{};
            while (objetivo != objetivo->padre->llaves_MBR_hijo[indice].second)
                indice++;
            if (objetivo->hoja)
                objetivo->padre->llaves_MBR_hijo[indice].first.redimensionar_tuplas(objetivo->llaves_tupla);
            else
                objetivo->padre->llaves_MBR_hijo[indice].first.redimensionar_mbrs(objetivo->llaves_MBR_hijo);
            ajustar_arbol(objetivo->padre, nullptr);
        }
        else
        {
            if (!objetivo->hoja)
            {
                if (objetivo->llaves_MBR_hijo[0].second->hoja)
                    for (auto &i : objetivo->llaves_MBR_hijo)
                    {
                        i.first.redimensionar_tuplas(i.second->llaves_tupla);
                    }
                else
                    for (auto &i : objetivo->llaves_MBR_hijo)
                    {
                        i.first.redimensionar_mbrs(i.second->llaves_MBR_hijo);
                    }
            }
        }
    }
    else
    {
        if (objetivo->padre == nullptr)
        {
            objetivo->padre = new R_Nodo(false);
            nuevo_nodo->padre = objetivo->padre;
            pair<R_MBR, R_Nodo *> llave_nodo_interno_1, llave_nodo_interno_2;
            llave_nodo_interno_1.second = objetivo;
            llave_nodo_interno_2.second = nuevo_nodo;
            if (objetivo->hoja)
            {
                llave_nodo_interno_1.first = R_MBR(objetivo->llaves_tupla);
                llave_nodo_interno_2.first = R_MBR(nuevo_nodo->llaves_tupla);
            }
            else
            {
                llave_nodo_interno_1.first = R_MBR(objetivo->llaves_MBR_hijo);
                llave_nodo_interno_2.first = R_MBR(nuevo_nodo->llaves_MBR_hijo);
            }
            objetivo->padre->llaves_MBR_hijo.push_back(llave_nodo_interno_1);
            objetivo->padre->llaves_MBR_hijo.push_back(llave_nodo_interno_2);

            root = objetivo->padre;
            root->padre = nullptr;
        }
        else
        {
            int i = 0;
            while (objetivo->padre->llaves_MBR_hijo[i].second != objetivo)
                i++;
            if (objetivo->hoja)
                objetivo->padre->llaves_MBR_hijo[i].first.redimensionar_tuplas(objetivo->llaves_tupla);
            else
                objetivo->padre->llaves_MBR_hijo[i].first.redimensionar_mbrs(objetivo->llaves_MBR_hijo);

            if (objetivo->padre->llaves_MBR_hijo.size() < M)
            {

                nuevo_nodo->padre = objetivo->padre;
                pair<R_MBR, R_Nodo *> llave_nodo_interno;
                llave_nodo_interno.second = nuevo_nodo;
                if (nuevo_nodo->hoja)
                    llave_nodo_interno.first = R_MBR(nuevo_nodo->llaves_tupla);
                else
                    llave_nodo_interno.first = R_MBR(nuevo_nodo->llaves_MBR_hijo);
                objetivo->padre->llaves_MBR_hijo.push_back(llave_nodo_interno);
                //
                ajustar_arbol(objetivo->padre, nullptr);
            }
            else
            {
                nuevo_nodo->padre = objetivo->padre;
                R_Nodo *particion_padre = partir_nodo_mbrs(objetivo->padre, nuevo_nodo);
                ajustar_arbol(objetivo->padre, particion_padre);
            }
        }
    }
}