#include "../include/Arbol_R.h"

double Arbol_R::obtener_sobrelapado_total(){
    double area_total=0, area_sobrelapada=0;

    set<int> seg_set_x, seg_set_y;

    queue<Nodo*> cola_nodos;
    cola_nodos.push(raiz);
    while(!cola_nodos.empty()){
        if(!cola_nodos.front()->hoja){
            for(Entrada* e: cola_nodos.front()->entradas){
                seg_set_x.insert(e->intervalos[0].i1);
                seg_set_x.insert(e->intervalos[0].i2);
                seg_set_y.insert(e->intervalos[1].i1);
                seg_set_y.insert(e->intervalos[1].i2);
                cola_nodos.push(dynamic_cast<Entrada_Interna*>(e)->puntero_hijo);
            }
        }
        cola_nodos.pop();
    }


    //////
    vector<int> segmento_x(seg_set_x.begin(), seg_set_x.end()), segmento_y(seg_set_y.begin(), seg_set_y.end());
    vector<vector<int>> matriz(seg_set_x.size() - 1, vector<int>(seg_set_y.size() - 1, 0));

    cola_nodos.push(raiz);
    for(int ix = 1; ix<segmento_x.size(); ix++){
        for(int iy = 1; iy<segmento_y.size(); iy++){
            
            while(!cola_nodos.empty()){
                if(!cola_nodos.front()->hoja){
                    for(Entrada* e: cola_nodos.front()->entradas){
                        if(e->dentro(segmento_x[ix-1], segmento_y[iy-1]) && e->dentro(segmento_x[ix], segmento_y[iy])){
                            matriz[ix-1][iy-1]++;
                        }
                    cola_nodos.push(dynamic_cast<Entrada_Interna*>(e)->puntero_hijo);
                    }
                }
                cola_nodos.pop();
            }
        }
    }
    //////

    for(int i = 0; i<segmento_x.size()-1; i++){
        for(int j = 0; j<segmento_y.size()-1; j++){
            if(matriz[i][j] > 1){
                area_sobrelapada += (segmento_x[i+1]-segmento_x[i])*(segmento_y[j+1]-segmento_y[j]);
            }
            area_total += (segmento_x[i+1]-segmento_x[i])*(segmento_y[j+1]-segmento_y[j]);
        }
    }

    return area_sobrelapada/area_total;
}
int Arbol_R::retornar_altura(){
    int altura = 0;
    Nodo* N = raiz;
    while(!N->hoja){
        N = dynamic_cast<Entrada_Interna*>(N->entradas.front())->puntero_hijo;
        altura++;
    }
    return altura;
}
double Arbol_R::obtener_sobrelapado(int Nivel){
    double area_total=0, area_sobrelapada=0;

    priority_queue<pair<int, Nodo*>, vector<pair<int, Nodo*>>, greater<pair<int,Nodo*>>> desciende;
    desciende.push({0,raiz});
    while(desciende.top().first + 1 < Nivel){
        Nodo* N = desciende.top().second;
        int nivel_N = desciende.top().first;
        desciende.pop();
        for(Entrada* e: N->entradas){
            desciende.push({nivel_N+1, dynamic_cast<Entrada_Interna*>(e)->puntero_hijo});
        }
    }

    vector<Nodo*> nodos;
    while(!desciende.empty()){
        // cout<<desciende.top().first<<endl;
        nodos.push_back(desciende.top().second);
        desciende.pop();
    }

    // cout<<"NIVEL: "<<Nivel<<endl;
    // cout<<"NUMERO DE NODOS: "<<nodos.size()<<endl;

    set<int> seg_set_x, seg_set_y;
    for(Nodo* n: nodos){
        for(Entrada* e: n->entradas){
            seg_set_x.insert(e->intervalos[0].i1);
            seg_set_x.insert(e->intervalos[0].i2);
            seg_set_y.insert(e->intervalos[1].i1);
            seg_set_y.insert(e->intervalos[1].i2);
        }
    }
    vector<int> segmento_x(seg_set_x.begin(), seg_set_x.end()), segmento_y(seg_set_y.begin(), seg_set_y.end());
    vector<vector<int>> matriz(seg_set_x.size() - 1, vector<int>(seg_set_y.size() - 1, 0));

    for(int ix = 1; ix<segmento_x.size(); ix++){
        for(int iy = 1; iy<segmento_y.size(); iy++){
            for(Nodo* n :nodos){
                for(Entrada* e: n->entradas){
                    if(e->dentro(segmento_x[ix-1], segmento_y[iy-1]) && e->dentro(segmento_x[ix], segmento_y[iy])){
                        matriz[ix-1][iy-1]++;
                    }
                }
            }
        }
    }

    for(int i = 0; i<segmento_x.size()-1; i++){
        for(int j = 0; j<segmento_y.size()-1; j++){
            if(matriz[i][j] > 1){
                area_sobrelapada += (segmento_x[i+1]-segmento_x[i])*(segmento_y[j+1]-segmento_y[j]);
            }
            area_total += (segmento_x[i+1]-segmento_x[i])*(segmento_y[j+1]-segmento_y[j]);
        }
    }

    return area_sobrelapada/area_total;
}

bool Arbol_R::buscar_exacto(const vector<Punto>& Ps){
    // MBR del objeto a bsucar
    Intervalo rectangulo[2]{{numeric_limits<int>::max(), numeric_limits<int>::min()}, {numeric_limits<int>::max(), numeric_limits<int>::min()}};
    for(int i = 0; i<Ps.size(); i++){
        rectangulo[0].i1 = min(rectangulo[0].i1, Ps[i].x);
        rectangulo[1].i1 = min(rectangulo[1].i1, Ps[i].y);
        rectangulo[0].i2 = max(rectangulo[0].i2, Ps[i].x);
        rectangulo[1].i2 = max(rectangulo[1].i2, Ps[i].y);
    }

    // Busqeuda con cola
    queue<Nodo*> cola;
    cola.push(raiz);
    // Mientras no haya cola vacía
    while(!cola.empty()){
        // Si el nodo de la cola es hoja
        if(cola.front()->hoja){
            // Iterar por todas sus entradas
            for(Entrada* e: cola.front()->entradas){
                // Verificar que el objeto sea igual
                Entrada_Hoja* et = dynamic_cast<Entrada_Hoja*>(e);
                if(Ps.size() != et->tuplas.size()){
                    continue;
                }

                bool iguales = true;
                for (int i_punto = 0; i_punto < Ps.size(); i_punto++){
                    if(Ps[i_punto].x != et->tuplas[i_punto].x || Ps[i_punto].y != et->tuplas[i_punto].y){
                        iguales = false;
                        break;
                    }
                }
                if(iguales){
                    return true;
                }                
            }
        }
        // Si es nodo interno
        else{
            // Iterar por entradas internas
            for(Entrada* e: cola.front()->entradas){
                Entrada_Interna* et = dynamic_cast<Entrada_Interna*>(e);
                // determinar si el MBR del objeto está dentro del mbr del nodo interno
                if(et->intervalos[0].i1 > rectangulo->i1 || et->intervalos[0].i2 < rectangulo->i2){
                    continue;
                }
                if(et->intervalos[1].i1 > rectangulo->i1 || et->intervalos[1].i2 < rectangulo->i2){
                    continue;
                }
                // Encolar los nodos hijos
                cola.push(et->puntero_hijo);
            }
        }
        cola.pop();
    }
    return false;
}

// FUNCION DEPRECADA
void nodo_entradas(Arbol_R* a){
    int n_nodos = 0, n_entradas = 0;
    function<void(Nodo*)> F = [&F, &n_nodos, &n_entradas](Nodo* N){
        n_nodos++;
        if(!N->hoja){
            for(Entrada* e: N->entradas){
                Entrada_Interna* ei = dynamic_cast<Entrada_Interna*>(e);
                F(ei->puntero_hijo);
            }
        }
        else
            n_entradas += N->entradas.size();
    };
    F(a->raiz);
    cout<<n_nodos<<'\t'<<n_entradas<<endl;
}


Arbol_R::Arbol_R():raiz(new Nodo{true, nullptr}), objetos(0) {}

Arbol_R::~Arbol_R(){
    // llamar la recursividad para liberar memoria dinamica desde la raiz
    destruir_recursivo(raiz);
}

void Arbol_R::insertar(vector<Punto> Ps) {
    // Convertir a entrada hoja
    Entrada_Hoja *E = new Entrada_Hoja{Ps};
    // I1
    // Escoger el nodo apropiado
    Nodo* L = this->escoger_hoja(E);
    // Definir lo que podría ser el nodo particionado
    Nodo* LL = nullptr;
    // I2
    // Si no hay overflow
    if(L->entradas.size() < M)
        // insertar al contenedor
        L->entradas.push_back(E);
    else
        // Splitear
        LL = this->partir_nodo(E, L);

    // I3
    // Insertar y ajustar en nodos superiores
    Nodo* partir_raiz = this->ajustar_arbol(L, LL);

    // I4
    // Si la raiz fue partida
    if(partir_raiz != nullptr){
        // Holdear valores antiguos de la raiz
        Nodo* raiz_temp = raiz;
        // Nueva raiz
        raiz = new Nodo{false, nullptr};
        // Entradas para la antigua raiz y su particion
        Entrada_Interna* e_raiz_temp = new Entrada_Interna, *e_partir_raiz = new Entrada_Interna;

        // ---- Obtener sus interevalos
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
        // ------ 

        // Definir padres
        partir_raiz->padre = raiz;
        raiz_temp->padre = raiz;
        // agregar al contenedor d ela nueva raiz
        raiz->entradas.push_back(e_raiz_temp);
        raiz->entradas.push_back(e_partir_raiz);
    }

    //Aumentar objetos
    objetos++;
}

// ELINIACION DEPRECADA
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

// Constructor de Objeto KNN
Arbol_R::Distante::Distante(Entrada* E, Punto P, Nodo* N): entrada(E), tupla(N){
    // Calcular la distancia euclideana de un punto hacia la entrada
    if(!N->hoja){
        if(P.x >= E->intervalos[0].i1 && P.x <= E->intervalos[0].i2 && P.y >= E->intervalos[1].i1 && P.y <= E->intervalos[1].i2)
            distancia = 0;
        else if(P.x >= E->intervalos[0].i1 && P.x <= E->intervalos[0].i2)
            distancia = min(abs(P.y-E->intervalos[1].i1), abs(P.y-E->intervalos[1].i2));
        else if(P.y >= E->intervalos[1].i1 && P.y <= E->intervalos[1].i2)
            distancia = min(abs(P.x-E->intervalos[0].i1), abs(P.x-E->intervalos[0].i2));
        else{
            distancia = min(
                sqrt(pow(P.y-E->intervalos[1].i1,2)+pow(P.x-E->intervalos[0].i1,2)), 
                min(
                    sqrt(pow(P.y-E->intervalos[1].i2,2)+pow(P.x-E->intervalos[0].i1,2)), 
                    min(
                        sqrt(pow(P.y-E->intervalos[1].i1,2)+pow(P.x-E->intervalos[0].i2,2)), 
                        sqrt(pow(P.y-E->intervalos[1].i2,2)+pow(P.x-E->intervalos[0].i2,2))))
                );
        }
    }
    else{
        Entrada_Hoja* EH = dynamic_cast<Entrada_Hoja*>(E);
        if(EH->tuplas.size() == 1){
            distancia = sqrt(pow(P.x-EH->tuplas[0].x,2)+pow(P.y-EH->tuplas[0].y,2));
        }
        else{
            double pm_x=0, pm_y=0;
            for(auto p: EH->tuplas){
                pm_x+=p.x;
                pm_y+=p.y;
            }
            pm_x/=EH->tuplas.size();
            pm_y/=EH->tuplas.size();
            distancia = sqrt(pow(P.x-pm_x,2)+pow(P.y-pm_y,2));
        }
    }
}

vector<Arbol_R::Distante> Arbol_R::buscar_k_vecinos(Punto P, int k){
    // Cola de prioridad para objetos KNN
    priority_queue<Arbol_R::Distante, deque<Arbol_R::Distante>, greater<Arbol_R::Distante>> knn_lista;
    // Encolar los objetos de la raiz
    for(int i = 0; i<raiz->entradas.size(); i++){
        knn_lista.push({raiz->entradas[i], P, raiz});
    }

    vector<Arbol_R::Distante> resultados;
    // Mientras haya obejtos por buscar y mientras la lista no esté llena
    while(resultados.size() < k && !knn_lista.empty()){
        // Si  es una entrada hoja, agregar a los resultados
        if(knn_lista.top().tupla->hoja){
            resultados.push_back(knn_lista.top());
            knn_lista.pop();
        }
        // Si es entrada interan
        else{
            Entrada_Interna* ET = dynamic_cast<Entrada_Interna*>(knn_lista.top().entrada);
            knn_lista.pop();
            // encolar todos sus hijos
            for(int i = 0; i<ET->puntero_hijo->entradas.size(); i++)
                knn_lista.push({ET->puntero_hijo->entradas[i], P, ET->puntero_hijo});
        }
    }
    // retornar los kNN
    return resultados;
}

void Arbol_R::eliminar_cercano(Punto P){
    // D1
    // Hallar el 1-NN
    Arbol_R::Distante cerca = buscar_k_vecinos(P, 1)[0];
    Entrada* E = cerca.entrada;
    Nodo* L = cerca.tupla;

    // D2
    for(int i = 0; i<L->entradas.size(); i++){
        if(L->entradas[i] == E){
            // Eliminar la entrada del objeto cercano
            L->entradas.erase(next(L->entradas.begin(), i));
            break;
        }
    }

    // D3
    // Arreglar los nodos superiores (underflow y reajuste de intervalos)
    condensar_cercano(L);

    // D4
    // Si la raiz solo tiene un solo hijo
    if(raiz->entradas.size() == 1 && !raiz->hoja){
        //  LA raiz será el  primer hijo
        raiz = dynamic_cast<Entrada_Interna*>(raiz->entradas[0])->puntero_hijo;
        raiz->padre = nullptr;
    }
}
