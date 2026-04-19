#include "utils.h"
#include "structures.h"
#include <iostream>
#include <thread>
#include <map>
#include <vector>
#include <mutex>
#include <chrono>
#include <ctime>

using namespace std;

// Estructura para info del Worker
struct WorkerInfo {
    int id;                 
    int socketId;           
    bool busy;              
    chrono::system_clock::time_point lastKA; 
};

map<int, WorkerInfo> workers;   
mutex mtxWorkers;
vector<string> systemLogs;      
mutex mtxLogs;

// Helper Logging (Se da hecho)
void addLog(string msg) {
    lock_guard<mutex> lock(mtxLogs);
    time_t now = time(0);
    string dt = ctime(&now);
    if (!dt.empty()) dt.pop_back(); 
    string entry = "[" + dt + "] " + msg;
    systemLogs.push_back(entry);
    cout << entry << endl;
}

// Timeout Cleaner (Extra)
void cleanerThread() {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        
        // TODO (EXTRA): Recorrer el mapa de workers.
        // Si (now - lastKA) > 10 segundos -> Eliminar worker y cerrar su socket.
        // Recordar usar mtxWorkers.
    }
}

void handleConnection(int clientId) {
    vector<unsigned char> buffer;
    recvMSG(clientId, buffer);
    if (buffer.empty()) { closeConnection(clientId); return; }

    MathMsgTypes tipo = unpack<MathMsgTypes>(buffer);

    if (tipo == MSG_LOGIN_WORKER) {
        int wId = unpack<int>(buffer);
        // TODO: Registrar nuevo worker en el mapa 'workers'.
        // Inicializar lastKA y busy=false.
        // NO cerrar la conexión (la necesitamos para enviar tareas).
        addLog("NUEVO WORKER: ID " + to_string(wId));
    }
    else if (tipo == MSG_WORKER_KA) { // Extra
        // TODO: Actualizar lastKA del worker.
        closeConnection(clientId);
    }
    else if (tipo == MSG_LOG_REQ) { // Extra
        // TODO: Enviar historial 'systemLogs' al cliente.
        closeConnection(clientId);
    }
    else if (tipo == MSG_CALC_REQ) {
        // PARTE PRINCIPAL: ROUTING
        
        int targetSocket = -1;
        int targetId = -1;

        // TODO: 1. Buscar un Worker libre (busy == false). Marcarlo como busy.

        if (targetSocket != -1) {
            // A. DESEMPAQUETAR PEDAGÓGICO
            // TODO: Desempaquetar A y B para verificar datos y loguear dimensiones.
            
            addLog("CALC: Asignado a Worker " + to_string(targetId));

            // B. REEMPAQUETAR
            // TODO: Volver a empaquetar MSG_CALC_REQ + A + B en un nuevo buffer.

            // 3. Enviar al Worker (targetSocket).
            
            // 4. Esperar respuesta del Worker (recvMSG).
            
            // 5. Enviar respuesta al Cliente (clientId).

            // 6. Liberar Worker (busy = false).
        } else {
            addLog("CALC RECHAZADO: Sin workers.");
        }
        closeConnection(clientId);
        return;
    }
    else {
        closeConnection(clientId);
    }
}

int main(int argc, char** argv) {
    int port = 9000;
    if (argc > 1) port = atoi(argv[1]);
    int srv = initServer(port);
    addLog("BROKER INICIADO: " + to_string(port));
    
    // Iniciar hilo limpiador (Descomentar al implementar extra)
    // std::thread thCleaner(cleanerThread);
    // thCleaner.detach();

    while (true) {
        while (!checkClient()) std::this_thread::sleep_for(std::chrono::milliseconds(50));
        int cliId = getLastClientID();
        std::thread t(handleConnection, cliId);
        t.detach();
    }
    close(srv);
    return 0;
}