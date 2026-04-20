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

    if(A.cols  != B.rows){
        cout << "Error: Dimensiones incompatibles" << endl;
        C.rows = 0;
        C.cols = 0;
        return C;
    } 
    
    for(int i = 0; i < A.rows; i++){
        for(int j = 0; j < B.cols; j++){
            for(int k = 0; k < A.cols; k++){
                setVal(C, i, j, getVal(C, i, j) + getVal(A, i, k) * getVal(B, k, j));
            }
        }
    }
    
    return C;
}