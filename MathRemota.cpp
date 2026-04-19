#include "Math.h"
#include "utils.h"
#include "structures.h"
#include <fstream>
#include <iostream>

// Variables globales para conexión (se definen en main_math.cpp)
extern string brokerIp;
extern int brokerPort;

Math::Math() {}

// --- IMPLEMENTACIÓN LOCAL (Disco del Cliente) ---
// SE PROPORCIONA HECHO
matrix_t Math::loadMatrix(string fileName) {
    matrix_t m;
    ifstream file(fileName);
    if (!file.is_open()) {
        cout << "Error abriendo fichero " << fileName << endl;
        m.rows = 0; m.cols = 0;
        return m;
    }
    file >> m.rows >> m.cols;
    m.data.resize(m.rows * m.cols);
    for (int i = 0; i < m.data.size(); i++) file >> m.data[i];
    file.close();
    return m;
}

void Math::saveMatrix(string fileName, matrix_t& m) {
    ofstream file(fileName);
    if (!file.is_open()) return;
    file << m.rows << " " << m.cols << endl;
    for (int i = 0; i < m.data.size(); i++) {
        file << m.data[i] << " ";
        if ((i + 1) % m.cols == 0) file << endl;
    }
    file.close();
}

// --- IMPLEMENTACIÓN REMOTA (RPC) ---

matrix_t Math::multiply(const matrix_t& A, const matrix_t& B) {
    matrix_t C;
    C.rows = 0; C.cols = 0;

    // TODO: Implementar el Proxy
    // 1. Conectar al Broker (initClient).
    // 2. Empaquetar MSG_CALC_REQ, Matriz A y Matriz B.
    // 3. Enviar (sendMSG).
    // 4. Recibir respuesta (recvMSG).
    // 5. Desempaquetar MSG_CALC_RES y Matriz C.
    // 6. Cerrar conexión.
    
    return C;
}

// EXTRA: Descarga de Logs
void Math::downloadLog() {
    // TODO (EXTRA):
    // 1. Conectar al Broker.
    // 2. Solicitar logs (MSG_LOG_REQ).
    // 3. Recibir respuesta, desempaquetar e imprimir por pantalla.
}