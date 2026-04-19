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
                // TODO: 1. Desempaquetar Matriz A (rows, cols, data).
                // TODO: 2. Desempaquetar Matriz B.
                
                // TODO: 3. Llamar a la lógica local: 
                // matrix_t C = mathLocal.multiply(A, B);

                // TODO: 4. Empaquetar MSG_CALC_RES y Matriz C.
                
                // TODO: 5. Enviar respuesta (sendMSG).
                
            } break;

            default:
                cout << "GestorCalculo: Mensaje desconocido." << endl;
                break;
        }
    }
}