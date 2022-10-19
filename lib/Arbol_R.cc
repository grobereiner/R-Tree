#include "../include/Arbol_R.h"

bool Arbol_R::comparar_x(Entrada *a, Entrada *b) {
    if(a->intervalos[0].i1 < b->intervalos[0].i1){
        if(a->intervalos[0].i2 <= b->intervalos[0].i2)
            return true;
        return false;
    }
    if(a->intervalos[0].i1 > b->intervalos[0].i1){
        if(a->intervalos[0].i2 < b->intervalos[0].i2)
            return false;
        return true;
    }
    return a->intervalos[0].i2 < b->intervalos[0].i2;
}

bool Arbol_R::comparar_y(Entrada *a, Entrada *b) {
    if(a->intervalos[1].i1 < b->intervalos[1].i1){
        if(a->intervalos[1].i2 <= b->intervalos[1].i2)
            return true;
        return false;
    }
    if(a->intervalos[1].i1 > b->intervalos[1].i1){
        if(a->intervalos[1].i2 < b->intervalos[1].i2)
            return false;
        return true;
    }
    return a->intervalos[1].i2 < b->intervalos[1].i2;
}

Arbol_R::Arbol_R():raiz(new Nodo{true, nullptr}) {}

void Arbol_R::insertar(vector<Punto> Ps) {
    Entrada_Hoja *E = new Entrada_Hoja{Ps};
    // I1
    Nodo* L = this->escoger_hoja(E);
    Nodo* LL = nullptr;
    // I2
    if(L->entradas.size() < M)
        L->entradas.push_back(E);
    else
        LL = this->partir_nodo(E, L);

    // I3
    Nodo* partir_raiz = this->ajustar_arbol(L, LL);

    // I4
    if(partir_raiz != nullptr){
        Nodo* raiz_temp = raiz;
        raiz = new Nodo{false, nullptr};
        Entrada_Interna* e_raiz_temp = new Entrada_Interna, *e_partir_raiz = new Entrada_Interna;

        e_raiz_temp->puntero_hijo = raiz_temp;
        e_raiz_temp->intervalos[0] = {numeric_limits<int>::max(), numeric_limits<int>::min()};
        e_raiz_temp->intervalos[1] = {numeric_limits<int>::max(), numeric_limits<int>::min()};

        e_partir_raiz->puntero_hijo = partir_raiz;
        e_partir_raiz->intervalos[0] = {numeric_limits<int>::max(), numeric_limits<int>::min()};
        e_partir_raiz->intervalos[1] = {numeric_limits<int>::max(), numeric_limits<int>::min()};

        for (Entrada *e: raiz_temp->entradas) {
            e_raiz_temp->intervalos[0].i1 = min(e_raiz_temp->intervalos[0].i1, e->intervalos[0].i1);
            e_raiz_temp->intervalos[1].i1 = min(e_raiz_temp->intervalos[1].i1, e->intervalos[1].i1);
            e_raiz_temp->intervalos[0].i2 = max(e_raiz_temp->intervalos[0].i2, e->intervalos[0].i2);
            e_raiz_temp->intervalos[1].i2 = max(e_raiz_temp->intervalos[1].i2, e->intervalos[1].i2);
        }

        for (Entrada *e: partir_raiz->entradas) {
            e_partir_raiz->intervalos[0].i1 = min(e_partir_raiz->intervalos[0].i1, e->intervalos[0].i1);
            e_partir_raiz->intervalos[1].i1 = min(e_partir_raiz->intervalos[1].i1, e->intervalos[1].i1);
            e_partir_raiz->intervalos[0].i2 = max(e_partir_raiz->intervalos[0].i2, e->intervalos[0].i2);
            e_partir_raiz->intervalos[1].i2 = max(e_partir_raiz->intervalos[1].i2, e->intervalos[1].i2);
        }

        partir_raiz->padre = raiz;
        raiz_temp->padre = raiz;
        raiz->entradas.push_back(e_raiz_temp);
        raiz->entradas.push_back(e_partir_raiz);
    }
}

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

void Arbol_R::eliminar(Punto P){

    // D1
    Nodo * H= hallar_hoja(raiz, P);
    if(H == nullptr)
        return;

    // D2 remover la coordenada o poligono que contiene la coordenada
    for (int i = 0; i < H->entradas.size(); i++)
    {
        Entrada_Hoja* EH = dynamic_cast<Entrada_Hoja*>(H->entradas[i]);

        ///////
        if (EH->tuplas.size() > 1)
        {
            if (!EH->dentro(P))
                continue;
            EH->tuplas.erase(next(EH->tuplas.begin(), i));
            break;
        }

        ///////
        bool en_x, en_y;
        en_x = P.x + RADIO >= EH->tuplas[0].x && P.x - RADIO <= EH->tuplas[0].x;
        en_y = P.y + RADIO >= EH->tuplas[0].y && P.y - RADIO <= EH->tuplas[0].y;
        if (!(en_x && en_y))
            continue;
        EH->tuplas.erase(next(EH->tuplas.begin(), i));
        break;
    }

    // D3 Condensar arbol
    deque<Nodo *> nodos_eliminados;
    condensar(H, nodos_eliminados);

    // D4 verificar si raiz tiene solo un hijo
    if (!raiz->hoja && raiz->entradas.size() == 1)
    {
        raiz = dynamic_cast<Entrada_Interna*>(raiz->entradas[0])->puntero_hijo;
        raiz->padre = nullptr;
    }
}

vector<Entrada_Hoja*> Arbol_R::buscar_k_vecinos(Punto P, int k){
    vector<Entrada_Hoja*> res;
    return res;
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