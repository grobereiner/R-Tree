#include "../include/Arbol_R.h"

Nodo* Arbol_R::ajustar_arbol(Nodo *L , Nodo *LL){
    // AT1
    // DEFINIR VARIABLES
    Nodo *N = L, *NN = LL;
    // AT2
    // MIENTRAS NO SE LLEGUE A LA RAIZ
    while(N != this->raiz) {
        // AT3
        // Obtener el padre 
        Nodo *P = N->padre;
        // Pasar por las entradas y reajustar MBRs
        for (Entrada *&E_N: P->entradas) {
            if (dynamic_cast<Entrada_Interna *>(E_N)->puntero_hijo == N) {
                E_N->intervalos[0] = {numeric_limits<int>::max(), numeric_limits<int>::min()};
                E_N->intervalos[1] = {numeric_limits<int>::max(), numeric_limits<int>::min()};
                for (Entrada *e: N->entradas) {
                    E_N->intervalos[0].i1 = min(E_N->intervalos[0].i1, e->intervalos[0].i1);
                    E_N->intervalos[1].i1 = min(E_N->intervalos[1].i1, e->intervalos[1].i1);
                    E_N->intervalos[0].i2 = max(E_N->intervalos[0].i2, e->intervalos[0].i2);
                    E_N->intervalos[1].i2 = max(E_N->intervalos[1].i2, e->intervalos[1].i2);
                }
                break;
            }
        }
        // AT4
        // Posible partición del padre
        Nodo *PP = nullptr;
        // SI hay una partición del nodo N
        if (NN != nullptr) {
            //---- Volverlo entrada y calcular su MBR
            Entrada *E_NN = new Entrada_Interna;
            dynamic_cast<Entrada_Interna *>(E_NN)->puntero_hijo = NN;
            E_NN->intervalos[0] = {numeric_limits<int>::max(), numeric_limits<int>::min()};
            E_NN->intervalos[1] = {numeric_limits<int>::max(), numeric_limits<int>::min()};
            for (Entrada *e: NN->entradas) {
                E_NN->intervalos[0].i1 = min(E_NN->intervalos[0].i1, e->intervalos[0].i1);
                E_NN->intervalos[1].i1 = min(E_NN->intervalos[1].i1, e->intervalos[1].i1);
                E_NN->intervalos[0].i2 = max(E_NN->intervalos[0].i2, e->intervalos[0].i2);
                E_NN->intervalos[1].i2 = max(E_NN->intervalos[1].i2, e->intervalos[1].i2);
            }
            // -----------

            // Si no provoca overflow el padre, insertarlo
            if (P->entradas.size() < M) {
                P->entradas.push_back(E_NN);
                NN->padre = P;
            }
            // Caso contrario, partir el nodo
            else
                PP = partir_nodo(E_NN, P);
        }

        // Subir un nivel, con el padre del nodo y la partición del padre como nuevo nivel base
        N = P;
        NN = PP;
    }
    return NN;
}

Nodo* Arbol_R::partir_nodo(Entrada *E, Nodo *L) {

    // DEFINIR SORT_SOPLIT
    Nodo* LL = new Nodo{L->hoja};
    vector<Entrada*> almacen = L->entradas;
    // ALmacen temporal de netradas
    almacen.push_back(E);

    // comparador de perimetro de distribuciones
    int perimetro_minimo = numeric_limits<int>::max();

    // Ordenar por x
    sort(almacen.begin(), almacen.end(), comparar_x);
    for(int i = m; i<=M-m+1; i++){
        // Obtener perimetros
        int distancia_x = abs(almacen[M]->intervalos[0].i2 - almacen[i]->intervalos[0].i1) + abs(almacen[i-1]->intervalos[0].i2 - almacen[0]->intervalos[0].i1);
        int distancia_y = abs(almacen[M]->intervalos[1].i2 - almacen[i]->intervalos[1].i1) + abs(almacen[i-1]->intervalos[1].i2 - almacen[0]->intervalos[1].i1);
        // Si el perimetro es menor que el comparador
        if(distancia_x*2 + distancia_y*2 < perimetro_minimo){
            // redefinir valores y las posibles entradas
            perimetro_minimo = distancia_x*2 + distancia_y*2;
            L->entradas = vector<Entrada*>(almacen.begin(), next(almacen.begin(), i));
			LL->entradas = vector<Entrada*>(next(almacen.begin(), i), almacen.end());
        }
    }
    // Ordenar por y
    sort(almacen.begin(), almacen.end(), comparar_y);
    for(int i = m; i<=M-m+1; i++){
        // Obtener perimetros
        int distancia_x = abs(almacen[M]->intervalos[0].i2 - almacen[i]->intervalos[0].i1) + abs(almacen[i-1]->intervalos[0].i2 - almacen[0]->intervalos[0].i1);
        int distancia_y = abs(almacen[M]->intervalos[1].i2 - almacen[i]->intervalos[1].i1) + abs(almacen[i-1]->intervalos[1].i2 - almacen[0]->intervalos[1].i1);
        // Si el perimetro es menor que el comparador
        if(distancia_x*2 + distancia_y*2 < perimetro_minimo){
            // redefinir valores y las posibles entradas
            perimetro_minimo = distancia_x*2 + distancia_y*2;
            L->entradas = vector<Entrada*>(almacen.begin(), next(almacen.begin(), i));
			LL->entradas = vector<Entrada*>(next(almacen.begin(), i), almacen.end());
        }
    }
    
    // En caso sea un nodo interno, tenemos que redefinir sus hijos nodos a sus nuevos padres
    if(!L->hoja){
        for(int i = 0; i<L->entradas.size(); i++)
            dynamic_cast<Entrada_Interna*>(L->entradas[i])->puntero_hijo->padre = L;
        for(int i = 0; i<LL->entradas.size(); i++)
            dynamic_cast<Entrada_Interna*>(LL->entradas[i])->puntero_hijo->padre = LL;
    }

    // Los entradas hoja no tienen padre
    // retornar nodo nuevo de partición
    return LL;
}

Nodo *Arbol_R::escoger_hoja(Entrada_Hoja *E) {
    // CL1
    // Empezar por la raiz
    Nodo* N = this->raiz;
    // CL2
    // Mientras no se encuentre un nodo hoja
    while(!N->hoja){
        // CL3
        Entrada *F = N->entradas[0];
        int minima_area = 1;
        minima_area *= max(N->entradas[0]->intervalos[0].i2, E->intervalos[0].i2) - min(N->entradas[0]->intervalos[0].i1, E->intervalos[0].i1);
        minima_area *= max(N->entradas[0]->intervalos[1].i2, E->intervalos[1].i2) - min(N->entradas[0]->intervalos[1].i1, E->intervalos[1].i1);
        // Hallar la entrada cuya expansión si fuese intertada la entrada, incremente menos
        for(int i = 1; i<N->entradas.size(); i++){
            int minima_area_local = 1;
            minima_area_local *= max(N->entradas[i]->intervalos[0].i2, E->intervalos[0].i2) - min(N->entradas[i]->intervalos[0].i1, E->intervalos[0].i1);
            minima_area_local *= max(N->entradas[i]->intervalos[1].i2, E->intervalos[1].i2) - min(N->entradas[i]->intervalos[1].i1, E->intervalos[1].i1);

            if(minima_area_local < minima_area){
                minima_area = minima_area_local;
                F = N->entradas[i];
            }
        }
        // CL4
        // bajar el hijo del mejor nodo
        N = dynamic_cast<Entrada_Interna*>(F)->puntero_hijo;
    }
    return N;
}

bool operator<(const Arbol_R::Distante &pd1, const Arbol_R::Distante &pd2){
    // Comparar < por distancia
    return pd1.distancia < pd2.distancia;
}
bool operator>(const Arbol_R::Distante &pd1, const Arbol_R::Distante &pd2){
    // Comparar > por distancia
    return pd1.distancia>pd2.distancia;
}
bool operator<=(const Arbol_R::Distante &pd1, const Arbol_R::Distante &pd2){
    // Comparar <= por distancia
    return pd1.distancia<=pd2.distancia;
}
bool operator>=(const Arbol_R::Distante &pd1, const Arbol_R::Distante &pd2){
    // Comparar >= por distancia
    return pd1.distancia>=pd2.distancia;
}

// FUNCION DEPRECADA
Nodo* Arbol_R::hallar_hoja(Nodo* E, Punto P){
    if (E->hoja)
    {
        for (auto tupla : E->entradas)
        {
            Entrada_Hoja* e = dynamic_cast<Entrada_Hoja*>(tupla);
            if (e->tuplas.size() == 1)
            {
                bool en_x, en_y;
                en_x = P.x-RADIO <= e->tuplas[0].x && P.x+RADIO >= e->tuplas[0].x;
                en_y = P.y-RADIO <= e->tuplas[0].y && P.y+RADIO >= e->tuplas[0].y;
                if (en_x && en_y)
                    return E;
            }
            else
            {
                if (e->dentro(P))
                    return E;
            }
        }
        return nullptr;
    }
    for (auto mbr_hijo : E->entradas)
    {        
        bool en_x, en_y;
        en_x = mbr_hijo->intervalos[0].i1<= P.x && P.x<= mbr_hijo->intervalos[0].i2;
        en_y = mbr_hijo->intervalos[1].i1<= P.y && P.y<= mbr_hijo->intervalos[1].i2;
        if (!(en_x && en_y))
            continue;
        Nodo *hallar = hallar_hoja(dynamic_cast<Entrada_Interna*>(mbr_hijo)->puntero_hijo, P);
        if (hallar)
            return hallar;
    }
    return nullptr;
}

// CONDENSACION DEPRECADA
void Arbol_R::condensar(Nodo* &H, deque<Nodo*> &NE){

    if (H != raiz)
    {
        // CT3 eliminar el nodo si tiene menos de m entradas
        if (H->entradas.size() < m)
        {

            NE.push_front(H);
            for (int i = 0; i < H->padre->entradas.size(); i++)
            {
                if (dynamic_cast<Entrada_Interna*>(H->padre->entradas[i])->puntero_hijo != H)
                    continue;
                H->padre->entradas.erase(next(H->padre->entradas.begin(), i));
                break;
            }
        }
        else
        {
            // CT4 reajustar los extremos del padre del nodo eliminado
            for (int i = 0; i < H->padre->entradas.size(); i++)
            {
                Entrada_Interna* ei = dynamic_cast<Entrada_Interna*>(H->padre->entradas[i]);
                if (ei->puntero_hijo == H)
                {
                    ei->intervalos[0] = {numeric_limits<int>::max(), numeric_limits<int>::min()};
                    ei->intervalos[1] = {numeric_limits<int>::max(), numeric_limits<int>::min()};

                    for(Entrada *e: H->entradas){
                        ei->intervalos[0].i1 = min(ei->intervalos[0].i1, e->intervalos[0].i1);
                        ei->intervalos[1].i1 = min(ei->intervalos[1].i1, e->intervalos[1].i1);
                        ei->intervalos[0].i2 = max(ei->intervalos[0].i2, e->intervalos[0].i2);
                        ei->intervalos[1].i2 = max(ei->intervalos[1].i2, e->intervalos[1].i2);
                    }
                }
            }
        }
        condensar(H->padre, NE);
    }
    else
    {
        for (int i = 0; i < NE.size(); i++)
        {
            if(NE[i]->hoja){
                for(auto NE_ent: NE[i]->entradas){
                    this->insertar(dynamic_cast<Entrada_Hoja*>(NE_ent)->tuplas);
                }
            }
            else{
                for(auto NE_ent: NE[i]->entradas){
                    this->ajustar_arbol(dynamic_cast<Entrada_Interna*>(NE_ent)->puntero_hijo->padre, NE[i]);
                }
            }
        }
    }
}

void Arbol_R::condensar_cercano(Nodo* L){
    // CT1
    // Incio, nodo hoja
    Nodo* N = L;
    // Vector de nodos eliminados
    vector<Nodo*> Q;
    // CT2
    while(N != raiz){
        // Padre de N
        Nodo* P = N->padre;
        int E_N;
        // Determinar si el nodo es eliminado
        bool eliminado = false;
        // Hallar al entrada que contiene al nodo
        for(int i = 0; i<P->entradas.size(); i++){
            if(dynamic_cast<Entrada_Interna*>(P->entradas[i])->puntero_hijo == N){
                E_N = i;
                break;
            }
        }
        // CT3
        // Si hay underflow
        if(N->entradas.size() < m){
            // eliminar el nood
            P->entradas.erase(next(P->entradas.begin(), E_N));
            eliminado = true;
            // Agregarlo a Q
            Q.push_back(N);
        }
        // CT4
        // Si hubo eliminaciones
        if(!eliminado){
            // Actualizar intervalor
            P->entradas[E_N]->intervalos[0] = {numeric_limits<int>::max(), numeric_limits<int>::min()};
            P->entradas[E_N]->intervalos[1] = {numeric_limits<int>::max(), numeric_limits<int>::min()};
            for(Entrada *e: N->entradas){
                P->entradas[E_N]->intervalos[0].i1 = min(P->entradas[E_N]->intervalos[0].i1, e->intervalos[0].i1);
                P->entradas[E_N]->intervalos[1].i1 = min(P->entradas[E_N]->intervalos[1].i1, e->intervalos[1].i1);
                P->entradas[E_N]->intervalos[0].i2 = max(P->entradas[E_N]->intervalos[0].i2, e->intervalos[0].i2);
                P->entradas[E_N]->intervalos[1].i2 = max(P->entradas[E_N]->intervalos[1].i2, e->intervalos[1].i2);
            }
        }
        // CT5
        // Subir al padre
        N = P;
    }
    // CT6
    for(int i = 0; i<Q.size(); i++){
        // Reinsertar todo lo eliminado
        insercion_recursiva(Q[i]);
    }
}

// Reinsertar todas las entradas del nodo
void Arbol_R::insercion_recursiva(Nodo* N){
    if(N->hoja){
        // Si es hoja, solamente insertar al arbol
        for(auto *e: N->entradas){
            this->insertar(dynamic_cast<Entrada_Hoja*>(e)->tuplas);
        }
    }
    else{
        for(auto *e: N->entradas){
            // Si no es hoja, descender e nivel y llamar a la función
            this->insercion_recursiva(dynamic_cast<Entrada_Interna*>(e)->puntero_hijo);
        }
    }
}