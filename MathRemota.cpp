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

    connection_t conn = initClient(brokerIp, brokerPort);
    if(!conn.alive){
        cout << "Error: no se pudo conectar al broker" << endl;
        return C;
    }
    // 2. Empaquetar MSG_CALC_REQ, Matriz A y Matriz B.

    vector<unsigned char> buffer;
    pack(buffer, MSG_CALC_REQ);

    pack(buffer, A.rows);
    pack(buffer, A.cols);
    packv(buffer, (int*)A.data.data(), A.data.size());

    pack(buffer, B.rows);
    pack(buffer, B.cols);
    packv(buffer, (int*)B.data.data(), B.data.size());

    // 3. Enviar 

    sendMSG(conn.serverId, buffer);

    // 4. Recibir respuesta 

    buffer.clear();
    recvMSG(conn.serverId, buffer);

    // 5. Desempaquetar MSG_CALC_RES y Matriz C.

    MathMsgTypes tipo = unpack<MathMsgTypes>(buffer);
    if(tipo == MSG_CALC_RES){
        C.rows = unpack<int>(buffer);
        C.cols = unpack<int>(buffer);
        C.data.resize(C.rows * C.cols);
        unpackv(buffer, C.data.data(), C.rows*C.cols);
    }

    // 6. Cerrar conexión.
    
    closeConnection(conn.serverId);
    return C;
}

// EXTRA: Descarga de Logs
void Math::downloadLog() {
    // TODO (EXTRA):
    // 1. Conectar al Broker.
    // 2. Solicitar logs (MSG_LOG_REQ).
    // 3. Recibir respuesta, desempaquetar e imprimir por pantalla.
}