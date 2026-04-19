#pragma once
#include "structures.h"
#include <string>

using namespace std;

class Math {
public:
    Math();
    
    // Operaciones Locales (Carga/Guardado en disco)
    matrix_t loadMatrix(string fileName);
    void saveMatrix(string fileName, matrix_t& m);
    
    // Operación Principal (Se distribuirá)
    matrix_t multiply(const matrix_t& A, const matrix_t& B);
    
    // Extra: Gestión
    void downloadLog();
};