#include "../include/R_Nodo.h"

R_Nodo::R_Nodo() : padre(nullptr) {}

R_Nodo::R_Nodo(bool _hoja) : hoja(_hoja)
{
    if (hoja)
        llaves_tupla.reserve(M);
    else
        llaves_MBR_hijo.reserve(M);
}