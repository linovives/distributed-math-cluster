#pragma once
#include "utils.h"
#include "structures.h"
#include "Math.h" 

class GestorCalculo {
public:
    // Función estática que atiende las peticiones (bucle infinito)
    static void atiendeCliente(int clientId);
};