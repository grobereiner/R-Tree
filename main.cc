#include <bits/stdc++.h>

using namespace std;

const int M = 4;
const int m = M / 2;

struct R_MBR;
struct R_Nodo;
struct R_Tree;

struct R_MBR
{
    R_MBR() = default;
    R_MBR(vector<pair<int, int>> tuplas)
    {
        this->redimensionar_tuplas(tuplas);
    }
    R_MBR(vector<pair<R_MBR, R_Nodo *>> mbrs)
    {
        this->redimensionar_mbrs(mbrs);
    }
    /*
    extremo[0]  ->  *-----
                    |    |
                    |    |
                    |    |
                    |    |
                    -----*  <- extremo[1]
    */
    pair<int, int> extremos[2];
    int agrandamiento(pair<int, int> tupla)
    {
        pair<int, int> temp_extremos[2]{extremos[0], extremos[1]};
        // para x
        if (tupla.first < temp_extremos[0].first && tupla.first < temp_extremos[1].first)
            temp_extremos[0].first = tupla.first;
        else if (tupla.first > temp_extremos[0].first && tupla.first > temp_extremos[1].first)
            temp_extremos[1].first = tupla.first;
        // para y
        if (tupla.second < temp_extremos[0].second && tupla.second < temp_extremos[1].second)
            temp_extremos[1].second = tupla.second;
        else if (tupla.second > temp_extremos[0].second && tupla.second > temp_extremos[1].second)
            temp_extremos[0].second = tupla.second;

        int area = abs((extremos[0].first - extremos[1].first) * (extremos[0].second - extremos[1].second));
        int nueva_area = abs((temp_extremos[0].first - temp_extremos[1].first) * (temp_extremos[0].second - temp_extremos[1].second));
        return nueva_area - area;
    }
    void redimensionar_tuplas(vector<pair<int, int>> tuplas)
    {
        extremos[0] = tuplas[0];
        extremos[1] = tuplas[0];
        for (auto i : tuplas)
        {
            if (i.first < extremos[0].first)
                extremos[0].first = i.first;
            if (i.first > extremos[1].first)
                extremos[1].first = i.first;
            if (i.second < extremos[1].second)
                extremos[1].second = i.second;
            if (i.second > extremos[0].second)
                extremos[0].second = i.second;
        }
    }

    void redimensionar_mbrs(vector<pair<R_MBR, R_Nodo *>> mbrs)
    {
        extremos[0] = mbrs[0].first.extremos[0];
        extremos[1] = mbrs[0].first.extremos[1];
        for (pair<R_MBR, R_Nodo *> i : mbrs)
        {
            if (i.first.extremos[0].first < extremos[0].first)
                extremos[0].first = i.first.extremos[0].first;
            if (i.first.extremos[1].first > extremos[1].first)
                extremos[1].first = i.first.extremos[1].first;
            if (i.first.extremos[1].second < extremos[1].second)
                extremos[1].second = i.first.extremos[1].second;
            if (i.first.extremos[0].second > extremos[0].second)
                extremos[0].second = i.first.extremos[0].second;
        }
    }
    bool dentro(pair<int, int> llave_tupla)
    {
        bool en_x = llave_tupla.first >= extremos[0].first && llave_tupla.first <= extremos[1].first;
        bool en_y = llave_tupla.second <= extremos[0].second && llave_tupla.second >= extremos[1].second;
        return en_x && en_y;
    }
};

struct R_Nodo
{

    R_Nodo() = default;
    R_Nodo(bool _hoja) : hoja(_hoja)
    {
        if (hoja)
            llaves_tupla.reserve(M);
        else
            llaves_MBR_hijo.reserve(M);
    }

    R_Nodo *padre = nullptr;
    // NODO HOJA
    vector<pair<int, int>> llaves_tupla;
    // NODO INTERNO
    vector<pair<R_MBR, R_Nodo *>> llaves_MBR_hijo;
    bool hoja;
};

class R_Tree
{
public:
    R_Tree() : root(new R_Nodo(true)) { root->padre = nullptr; }
    void print_desmos()
    {
        int tupla_id = 0;
        print_desmos(root, tupla_id);
        cout << '\n';
    }
    void insercion(pair<int, int> llave_tupla)
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
    void eliminacion(pair<int, int> llave_tupla)
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
        condensar(obj);

        // D4 verificar si raiz tiene solo un hijo
        if (!root->hoja && root->llaves_MBR_hijo.size() == 1)
        {
            root = root->llaves_MBR_hijo[0].second;
            root->padre = nullptr;
        }
    }

private:
    void condensar(R_Nodo *&nodo, deque<R_Nodo *> nodos_eliminados = {})
    {
        // CT2 verificar que no sea raiz
        if (nodo != root)
        {
            // CT3 eliminar el nodo si tiene menos de m entradas
            if (nodo->llaves_tupla.size() < m)
            {

                nodos_eliminados.push_front(nodo);
                for (int i = 0; i < nodo->padre->llaves_MBR_hijo.size(); i++)
                {
                    if (nodo->padre->llaves_MBR_hijo[i].second != nodo)
                        continue;
                    nodo->padre->llaves_MBR_hijo.erase(next(nodo->padre->llaves_MBR_hijo.begin(), i));
                    break;
                }

                // CT4 reajustar los extremos del padre del nodo eliminado
                for (int i = 0; i < nodo->padre->padre->llaves_MBR_hijo.size(); i++)
                {
                    if(nodo->padre->padre->llaves_MBR_hijo[i].second = nodo->padre){
                        nodo->padre->padre->llaves_MBR_hijo[i].first.redimensionar_mbrs(nodo->padre->llaves_MBR_hijo);
                        break;
                    }
                }
            }
            condensar(nodo->padre, nodos_eliminados); 
        }
        else{
            for(int i = 0; i<nodos_eliminados.size(); i++){
                insertar_entradas(nodos_eliminados[i]);
            }
        }
    }

    void insertar_entradas(R_Nodo *nodo){
        if(nodo->hoja){
            for(auto i : nodo->llaves_tupla){
                insercion(i);
            }
        }
        else{
            for(auto i: nodo->llaves_MBR_hijo){
                insertar_entradas(i.second);
            }
        }
    }

    R_Nodo *hallar_hoja(R_Nodo *nodo, pair<int, int> llave_tupla)
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
    void print_desmos(R_Nodo *nodo, int &tupla_id)
    {

        if (nodo->hoja)
        {
            for (auto i : nodo->llaves_tupla)
                printf("P_{%d} = (%d, %d)\n", tupla_id++, i.first, i.second);
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
    static bool comparar_x_tupla(pair<int, int> a, pair<int, int> b) { return a.first < b.first; }
    static bool comparar_x_mbr(pair<R_MBR, R_Nodo *> a, pair<R_MBR, R_Nodo *> b)
    {
        if (a.first.extremos[0].first < b.first.extremos[0].first)
            return true;
        if (a.first.extremos[0].first == b.first.extremos[0].first)
            return a.first.extremos[1].first < b.first.extremos[1].first;
        return false;
    }
    static bool comparar_y_tupla(pair<int, int> a, pair<int, int> b) { return a.second < b.second; }
    static bool comparar_y_mbr(pair<R_MBR, R_Nodo *> a, pair<R_MBR, R_Nodo *> b)
    {
        if (a.first.extremos[1].second < b.first.extremos[1].second)
            return true;
        if (a.first.extremos[1].second == b.first.extremos[1].second)
            return a.first.extremos[0].second < b.first.extremos[1].second;
        return false;
    }

    R_Nodo *partir_nodo_mbrs(R_Nodo *&nodo, R_Nodo *otro_nodo_interno)
    {
        // int cota_inferior = ceil(0.4 * m), cota_superior = M - ceil(0.4 * m);
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

    R_Nodo *partir_nodo_tuplas(R_Nodo *&nodo, pair<int, int> llave_tupla)
    {
        // int cota_inferior = ceil(0.4 * m), cota_superior = M - ceil(0.4 * m);
        int cota_inferior = m, cota_superior = M - m;
        vector<pair<int, int>> almacen{nodo->llaves_tupla};
        almacen.push_back(llave_tupla);

        vector<pair<int, int>> S1, S2;
        int perimetro_minimo = numeric_limits<int>::max();
        // Para X
        sort(almacen.begin(), almacen.end(), comparar_x_tupla);
        for (int i = cota_inferior; i <= cota_superior; i++)
        {
            vector<pair<int, int>> tS1(almacen.begin(), next(almacen.begin(), i)), tS2(next(almacen.begin(), i), almacen.end());
            int perimetro1 = 2 * abs(tS1[0].first - tS1[tS1.size() - 1].first) + 2 * abs(tS1[0].second - tS1[tS1.size() - 1].second);
            int perimetro2 = 2 * abs(tS2[0].first - tS2[tS2.size() - 1].first) + 2 * abs(tS2[0].second - tS2[tS2.size() - 1].second);
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
            vector<pair<int, int>> tS1(almacen.begin(), next(almacen.begin(), i)), tS2(next(almacen.begin(), i), almacen.end());
            int perimetro1 = 2 * abs(tS1[0].first - tS1[tS1.size() - 1].first) + 2 * abs(tS1[0].second - tS1[tS1.size() - 1].second);
            int perimetro2 = 2 * abs(tS2[0].first - tS2[tS2.size() - 1].first) + 2 * abs(tS2[0].second - tS2[tS2.size() - 1].second);
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

    R_Nodo *escoger_nodo(R_Nodo *nodo, pair<int, int> llave_tupla)
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

    void ajustar_arbol(R_Nodo *&objetivo, R_Nodo *nuevo_nodo)
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

                    /*i = 0;
                    while(objetivo->padre->llaves_MBR_hijo[i].second != objetivo)
                        i++;
                    if(objetivo->hoja)
                        objetivo->padre->llaves_MBR_hijo[i].first.redimensionar_tuplas(objetivo->llaves_tupla);
                    else
                        objetivo->padre->llaves_MBR_hijo[i].first.redimensionar_mbrs(objetivo->llaves_MBR_hijo);*/

                    ajustar_arbol(objetivo->padre, particion_padre);
                }
            }
        }
    }

    R_Nodo *root;
};

int main()
{
    R_Tree arbolito;
    /* arbolito.insercion({-15,7});
    arbolito.insercion({0,5});
    arbolito.insercion({-13,1});
    arbolito.insercion({-2,1});
    arbolito.insercion({8,8});
    arbolito.insercion({11,-4});
    arbolito.insercion({5,-1});
    arbolito.insercion({16,-9});
    arbolito.insercion({-16,2});
    arbolito.insercion({8,7});
    arbolito.insercion({0,9});
    arbolito.insercion({-2,6});
    arbolito.insercion({-6,3});
    arbolito.insercion({7,-3});
    arbolito.insercion({-4,4});
    arbolito.insercion({15,5});
    arbolito.insercion({-8,-7});
    arbolito.insercion({14,-3});
    arbolito.insercion({4,1});
    arbolito.insercion({-19,8});
    arbolito.insercion({-11,-1});
    arbolito.insercion({-4,0});
    arbolito.insercion({-6,3});
    arbolito.insercion({-19,-8});
    arbolito.insercion({-6,1}); */

    arbolito.insercion({3, -6});
    arbolito.insercion({16, 7});
    arbolito.insercion({6, 9});
    arbolito.insercion({0, -3});
    arbolito.insercion({15, -9});
    arbolito.insercion({-3, -3});
    arbolito.insercion({16, -1});
    arbolito.insercion({-3, 1});
    arbolito.insercion({-7, -7});
    arbolito.insercion({6, 8});
    arbolito.insercion({-9, 0});
    arbolito.insercion({16, 4});
    arbolito.insercion({15, 3});
    arbolito.insercion({16, 5});
    arbolito.insercion({8, -3});
    arbolito.insercion({-4, 0});
    arbolito.insercion({-19, 4});
    arbolito.insercion({-14, 8});
    arbolito.insercion({11, -8});
    arbolito.insercion({13, 7});
    arbolito.insercion({-2, -5});
    arbolito.insercion({2, 1});
    arbolito.insercion({16, 3});
    arbolito.insercion({5, 5});
    arbolito.insercion({-1, -3});

    arbolito.print_desmos();
    return 0;
}