#include "Math.h"

Math::Math() {}

// Funciones no usadas en el servidor (vacías)
matrix_t Math::loadMatrix(string fileName) { return matrix_t(); }
void Math::saveMatrix(string fileName, matrix_t& m) {}
void Math::downloadLog() {}

// LÓGICA DE NEGOCIO (CPU Bound)
matrix_t Math::multiply(const matrix_t& A, const matrix_t& B) {
    matrix_t C;
    C.rows = A.rows;
    C.cols = B.cols;
    C.data.resize(C.rows * C.cols, 0);

    // TODO: Implementar la multiplicación de matrices AxB = C
    // 1. Comprobar dimensiones (A.cols == B.rows).
    // 2. Usar 3 bucles anidados.
    // 3. Usar helpers getVal(A, i, k) y setVal(C, i, j, val).
    
    return C;
}