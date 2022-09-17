#include "../include/R_Tree.h"

int main()
{
    R_Tree arbolito;

    arbolito.insercion({3, -6});
    arbolito.insercion({16, 7});
    arbolito.insercion({6, 9});
    arbolito.insercion({0, -3});
    arbolito.insercion({15, -9});
    arbolito.insercion({-3, -3});
    arbolito.insercion({16, -1});
    arbolito.insercion({-3, 1});
    arbolito.insercion({-7,-7});
    arbolito.insercion({6,8});
    arbolito.insercion({-9,0});
    arbolito.insercion({16,4});
    arbolito.insercion({15,3});
    arbolito.insercion({16,5});
    arbolito.insercion({8,-3});
    arbolito.insercion({-4,0});
    arbolito.insercion({-19,4});

    arbolito.eliminacion({0,-3});
    arbolito.eliminacion({-3,-3});
    arbolito.eliminacion({3,-6});

    arbolito.print_desmos();
    return 0;
}