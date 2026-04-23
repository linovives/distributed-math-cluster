#include "utils.h"
#include "structures.h"
#include "GestorCalculo.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <unistd.h>

using namespace std;

string brokerIp;
int brokerPort;
int myId;
bool running = true;

// EXTRA: Hilo de Keep Alive
void keepAliveThread() {
    while (running) {
        // TODO (EXTRA): Implementar el envío periódico de señales de vida.
        // 1. Dormir el hilo 2 segundos.
        std::this_thread::sleep_for(std::chrono::seconds(2));
        // 2. Crear una conexión efímera con initClient.
        // 3. Enviar mensaje MSG_WORKER_KA con myId.
        // 4. Cerrar conexión.

        connection_t conn = initClient(brokerIp, brokerPort);
        if(conn.alive){
            vector <unsigned char> buffer;
            pack(buffer, MSG_WORKER_KA);
            pack(buffer, myId);
            sendMSG(conn.serverId, buffer);
            closeConnection(conn.serverId);
        }
        
        std::this_thread::sleep_for(std::chrono::seconds(2)); // Placeholder para que no consuma CPU al 100%
    }
}

int main(int argc, char** argv) {
    if (argc != 3) {
        cout << "Usage: server <BROKER_IP> <BROKER_PORT>" << endl;
        return 0;
    }
    brokerIp = argv[1];
    brokerPort = atoi(argv[2]);
    
    // Generar ID aleatorio
    srand(time(NULL) + getpid());
    myId = rand() % 10000;

    cout << "--- WORKER SERVER (ID: " << myId << ") ---" << endl;

    // 1. Conexión Persistente (Para recibir Tareas)
    auto conn = initClient(brokerIp, brokerPort);
    if (!conn.alive) {
        cout << "Error conectando al Broker." << endl;
        return 1;
    }

    // 2. Registro en el Broker
    vector<unsigned char> buffer;
    pack(buffer, MSG_LOGIN_WORKER);
    pack(buffer, myId);
    sendMSG(conn.serverId, buffer);

    // 3. Iniciar hilo Keep Alive (Detached)
    std::thread thKA(keepAliveThread);
    thKA.detach();

    // 4. Entregar control al Gestor (Bucle infinito)
    GestorCalculo::atiendeCliente(conn.serverId);

    closeConnection(conn.serverId);
    return 0;
}