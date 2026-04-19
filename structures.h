#pragma once
#include <vector>
#include <string>
#include <iostream>

using namespace std;

// Estructura de Matriz
typedef struct {
    int rows;
    int cols;
    vector<int> data;
} matrix_t;

// Tipos de Mensaje
typedef enum {
    MSG_LOGIN_WORKER,   // Worker -> Broker: "Hola, soy un worker"
    MSG_WORKER_KA,      // Worker -> Broker: "Sigo vivo" (EXTRA)
    MSG_CALC_REQ,       // Cliente -> Broker -> Worker: "Calcula esto"
    MSG_CALC_RES,       // Worker -> Broker -> Cliente: "Aquí está el resultado"
    MSG_LOG_REQ,        // Cliente -> Broker: "Dame los logs" (EXTRA)
    MSG_LOG_RES,        // Broker -> Cliente: "Toma los logs"
    MSG_ERROR           // Error genérico
} MathMsgTypes;

// Helpers para acceso a matriz (Renombrados para evitar conflictos)
inline int getVal(const matrix_t& m, int r, int c) {
    if(r < 0 || r >= m.rows || c < 0 || c >= m.cols) return 0;
    return m.data[r * m.cols + c];
}

inline void setVal(matrix_t& m, int r, int c, int val) {
    if(r >= 0 && r < m.rows && c >= 0 && c < m.cols)
        m.data[r * m.cols + c] = val;
}