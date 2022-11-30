#include"../include/Arbol_R.h"
#include"../include/Interfaz.h"
#include<random>
#include<fstream>
#include<chrono>

using namespace std;
using namespace std::chrono;

int x = N, y = N;
random_device rd;

vector<Punto> generar_poligono(){
    int l_lado = (x*(rd()%64 + 21))/10000.f;
    int n_lados = rd()%3 + 3;

    Punto trasladar{rd()%(x-100) + 50, rd()%(y-100) + 50};

    vector<Punto> P;
    if(n_lados == 3){
        P = {{0,0}, {l_lado, 0}, {static_cast<int>(round(l_lado/2.f)), static_cast<int>(round(l_lado*sqrt(3)/2.f))}};
    }
    else if(n_lados == 4){
        P = {{0,0},{0,l_lado},{l_lado, l_lado},{l_lado, 0}};
    }
    else{
        P = {{0,0},{0,l_lado},{l_lado, l_lado},{l_lado, 0}};
    }

    for(int i = 0; i<P.size(); i++){
        P[i].x += trasladar.x;
        P[i].y += trasladar.y;
    }

    return P;
}

void test_insercion(){
    ofstream of("R_Tree_Insercion.csv", ofstream::out);
    for(int i = 1; i<=499; i++){
        of<<i<<',';
    }
    of<<500<<'\n';
    for(int t = 0; t<100; t++){
        Arbol_R* a = new Arbol_R;
        for(int b =0; b<500; b++){
            auto start = high_resolution_clock::now();
            for(int p = 0; p<10; p++){

                a->insertar(generar_poligono());
            }
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<nanoseconds>(stop - start);
            if(b != 499)
                of<<duration.count()<<',';
            else
                of<<duration.count()<<'\n';

            // nodo_entradas(a);
            
        }
        delete a;
    }
    of.close();
}

void test_borrado(){
    ofstream of("T_D.csv", ofstream::out);
    for(int i = 0; i<100; i++){
        Arbol_R* a = new Arbol_R;
        

        for(int i =0; i<5000; i++){
            a->insertar(generar_poligono());
        }

        for(int i = 0; i<5; i++){
            auto start = high_resolution_clock::now();
            for(int i = 0; i<1000; i++){
                Punto ubicacion{rd()%2048, rd()%2048};
                a->eliminar_cercano(ubicacion);
            }
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<nanoseconds>(stop - start);
            if(i != 4)
                of<<duration.count()<<',';
            else
                of<<duration.count()<<'\n';
        }

        // nodo_entradas(a);
            
        
        delete a;
    }
    of.close();
}

void test_busqueda(){
    int n_figuras_por_batch = 1000;
    // int n_figuras_por_batch = 10;
    ofstream of("T_B.csv", ofstream::out);
    vector<Punto> polis[n_figuras_por_batch];
    for (int i = 1; i <= 5; i++)
    {
        of<<i;
        if(i != 5)
            of<<',';
    }
    of<<'\n';

    for(int t = 0; t<100; t++){
        Arbol_R* a = new Arbol_R;
        for(int b = 0; b<5; b++){
            for(int n_p = 0; n_p<n_figuras_por_batch; n_p++){
                polis[n_p] = generar_poligono();
                a->insertar(polis[n_p]);
            }
            auto start = high_resolution_clock::now();
            for(int n_p = 0; n_p<n_figuras_por_batch; n_p++){
                bool hallado = a->buscar_exacto(polis[n_p]);

                // if(!hallado){
                //     cout<<"No hallado";
                //     delete a;
                //     return;
                // }
            }
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<nanoseconds>(stop - start);
            
            if(b != 4)
                of<<duration.count()<<',';
            else
                of<<duration.count()<<'\n';

        }
        /* Interfaz i;
        i.setArbol(a);
        i.ejecutar(); */
        delete a;
        cout<<t<<'\n';
    }
    of<<'\n';
    of.close();    
}

void test_knn(){
    ofstream of("T_K.csv", ofstream::out);
    for(int i = 1; i<=5; i++){
        of<<"T"<<i<<"K=1"<<',';
        of<<"T"<<i<<"K=5"<<',';
        of<<"T"<<i<<"K=20";
        if(i != 5)
            of<<',';
    }
    of<<'\n';

    int ks[3]{1,5,20};

    for(int i = 0; i<100; i++){
        Arbol_R* a = new Arbol_R;
        for(int j =0; j<5; j++){
            for(int k = 0; k<1000; k++){
                a->insertar(generar_poligono());
            }
            for(int k = 0; k<3; k++){
                auto start = high_resolution_clock::now();
                Punto ubicacion{rd()%2048, rd()%2048};
                a->buscar_k_vecinos(ubicacion, ks[k]);
                auto stop = high_resolution_clock::now();
                auto duration = duration_cast<nanoseconds>(stop - start);
                if(k == 2 && j==4)
                    of<<duration.count()<<'\n';
                else
                    of<<duration.count()<<',';

            }


            // nodo_entradas(a);
            
        }
        delete a;
    }

    of.close();
}

int main(int argc, char const *argv[])
{
    test_busqueda();
    return 0;
}
