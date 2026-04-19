#include "GestorCalculo.h"

void GestorCalculo::atiendeCliente(int clientId) {
    bool salir = false;
    vector<unsigned char> buffer;
    
    // Instancia local de la lógica (usará Math.cpp al compilar el server)
    Math mathLocal; 

    while (!salir) {
        // 1. Recibir mensaje
        buffer.clear();
        recvMSG(clientId, buffer);

        if (buffer.size() == 0) {
            salir = true; 
            break;
        }

        // 2. Desempaquetar tipo
        MathMsgTypes tipo = unpack<MathMsgTypes>(buffer);

        switch (tipo) {
            case MSG_CALC_REQ: {
                // 1. Desempaquetar Matriz A (rows, cols, data).
                matrix_t A;
                A.rows = unpack<int>(buffer);
                A.cols = unpack<int>(buffer);
                A.data.resize(A.rows * A.cols);
                unpackv(buffer, A.data.data(), A.rows * A.cols);

                // 2. Desempaquetar Matriz B.
                matrix_t B;
                B.rows = unpack<int>(buffer);
                B.cols = unpack<int>(buffer);
                B.data.resize(B.rows * B.cols);
                unpackv(buffer, B.data.data(), B.rows * B.cols);


                // 3. Calcular:
                matrix_t C = mathLocal.multiply(A, B);

                // 4. Empaquetar MSG_CALC_RES y Matriz C.
                buffer.clear();
                pack(buffer, MSG_CALC_RES);
                pack(buffer, C.rows);
                pack(buffer, C.cols);
                packv(buffer, C.data.data(), C.data.size());
                
                // 5. Enviar respuesta 
                sendMSG(clientId, buffer);
                salir = true;
                
            } break;

            default:
                cout << "GestorCalculo: Mensaje desconocido." << endl;
                break;
        }
    }
}