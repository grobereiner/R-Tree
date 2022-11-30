#include "../include/Arbol_R.h"

Nodo* Arbol_R::ajustar_arbol(Nodo *L , Nodo *LL){
    // AT1
    Nodo *N = L, *NN = LL;
    // AT2
    while(N != this->raiz) {
        // AT3
        Nodo *P = N->padre;
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
        Nodo *PP = nullptr;
        if (NN != nullptr) {
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
            if (P->entradas.size() < M) {
                P->entradas.push_back(E_NN);
                NN->padre = P;
            }
            else
                PP = partir_nodo(E_NN, P);
        }

        N = P;
        NN = PP;
    }
    return NN;
}

Nodo* Arbol_R::partir_nodo(Entrada *E, Nodo *L) {
    Nodo* LL = new Nodo{L->hoja};
    vector<Entrada*> almacen = L->entradas;
    almacen.push_back(E);

    int perimetro_minimo = numeric_limits<int>::max();

    sort(almacen.begin(), almacen.end(), comparar_x);
    for(int i = m; i<=M-m+1; i++){
        int distancia_x = abs(almacen[M]->intervalos[0].i2 - almacen[i]->intervalos[0].i1) + abs(almacen[i-1]->intervalos[0].i2 - almacen[0]->intervalos[0].i1);
        int distancia_y = abs(almacen[M]->intervalos[1].i2 - almacen[i]->intervalos[1].i1) + abs(almacen[i-1]->intervalos[1].i2 - almacen[0]->intervalos[1].i1);
        if(distancia_x*2 + distancia_y*2 < perimetro_minimo){
            perimetro_minimo = distancia_x*2 + distancia_y*2;
            L->entradas = vector<Entrada*>(almacen.begin(), next(almacen.begin(), i));
			LL->entradas = vector<Entrada*>(next(almacen.begin(), i), almacen.end());
        }
    }
    sort(almacen.begin(), almacen.end(), comparar_y);
    for(int i = m; i<=M-m+1; i++){
        int distancia_x = abs(almacen[M]->intervalos[0].i2 - almacen[i]->intervalos[0].i1) + abs(almacen[i-1]->intervalos[0].i2 - almacen[0]->intervalos[0].i1);
        int distancia_y = abs(almacen[M]->intervalos[1].i2 - almacen[i]->intervalos[1].i1) + abs(almacen[i-1]->intervalos[1].i2 - almacen[0]->intervalos[1].i1);
        if(distancia_x*2 + distancia_y*2 < perimetro_minimo){
            perimetro_minimo = distancia_x*2 + distancia_y*2;
            L->entradas = vector<Entrada*>(almacen.begin(), next(almacen.begin(), i));
			LL->entradas = vector<Entrada*>(next(almacen.begin(), i), almacen.end());
        }
    }
    
    if(!L->hoja){
        for(int i = 0; i<L->entradas.size(); i++)
            dynamic_cast<Entrada_Interna*>(L->entradas[i])->puntero_hijo->padre = L;
        for(int i = 0; i<LL->entradas.size(); i++)
            dynamic_cast<Entrada_Interna*>(LL->entradas[i])->puntero_hijo->padre = LL;
    }
    return LL;
}

Nodo *Arbol_R::escoger_hoja(Entrada_Hoja *E) {
    // CL1
    Nodo* N = this->raiz;
    // CL2
    while(!N->hoja){
        // CL3
        Entrada *F = N->entradas[0];
        int minima_area = 1;
        minima_area *= max(N->entradas[0]->intervalos[0].i2, E->intervalos[0].i2) - min(N->entradas[0]->intervalos[0].i1, E->intervalos[0].i1);
        minima_area *= max(N->entradas[0]->intervalos[1].i2, E->intervalos[1].i2) - min(N->entradas[0]->intervalos[1].i1, E->intervalos[1].i1);
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
        N = dynamic_cast<Entrada_Interna*>(F)->puntero_hijo;
    }
    return N;
}

bool operator<(const Arbol_R::Distante &pd1, const Arbol_R::Distante &pd2){
    return pd1.distancia < pd2.distancia;
}
bool operator>(const Arbol_R::Distante &pd1, const Arbol_R::Distante &pd2){
    return pd1.distancia>pd2.distancia;
}
bool operator<=(const Arbol_R::Distante &pd1, const Arbol_R::Distante &pd2){
    return pd1.distancia<=pd2.distancia;
}
bool operator>=(const Arbol_R::Distante &pd1, const Arbol_R::Distante &pd2){
    return pd1.distancia>=pd2.distancia;
}

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
    Nodo* N = L;
    vector<Nodo*> Q;
    // CT2
    while(N != raiz){
        Nodo* P = N->padre;
        int E_N;
        bool eliminado = false;
        for(int i = 0; i<P->entradas.size(); i++){
            if(dynamic_cast<Entrada_Interna*>(P->entradas[i])->puntero_hijo == N){
                E_N = i;
                break;
            }
        }
        // CT3
        if(N->entradas.size() < m){
            P->entradas.erase(next(P->entradas.begin(), E_N));
            eliminado = true;
            Q.push_back(N);
        }
        // CT4
        if(!eliminado){
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
        N = P;
    }
    // CT6
    for(int i = 0; i<Q.size(); i++){
        insercion_recursiva(Q[i]);
    }
}

void Arbol_R::insercion_recursiva(Nodo* N){
    if(N->hoja){
        for(auto *e: N->entradas){
            this->insertar(dynamic_cast<Entrada_Hoja*>(e)->tuplas);
        }
    }
    else{
        for(auto *e: N->entradas){
            this->insercion_recursiva(dynamic_cast<Entrada_Interna*>(e)->puntero_hijo);
        }
    }
}