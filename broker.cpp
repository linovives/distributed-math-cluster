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
        WorkerInfo w;
        w.id = wId;
        w.socketId = clientId;
        w.busy = false;
        w.lastKA = chrono::system_clock::now();
        mtxWorkers.lock();
        workers[wId] = w;
        mtxWorkers.unlock();
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

        // Buscar un Worker libre (busy == false). Marcarlo como busy.
        mtxWorkers.lock();

        for(auto& [id, w] : workers){
            if(!w.busy){
                w.busy = true;
                targetSocket = w.socketId;
                targetId = w.id;
                break;
            }
        }
        mtxWorkers.unlock();

        if (targetSocket != -1) {
            // A. Desempaquetar para loguear
            int rA = unpack<int>(buffer);
            int cA = unpack<int>(buffer);
            int sizeA = rA * cA;
            vector<int> dataA(sizeA);
            unpackv(buffer, dataA.data(), sizeA);

            int rB = unpack<int>(buffer);
            int cB = unpack<int>(buffer);
            int sizeB = rB * cB;
            vector<int> dataB(sizeB);
            unpackv(buffer, dataB.data(), sizeB);

            addLog("CALC: Matriz " + to_string(rA) + "x" + to_string(cA) + " asignada a Worker " + to_string(targetId));

            // B. Reempaquetar todo de nuevo
            vector<unsigned char> fwd;
            pack(fwd, MSG_CALC_REQ);
            pack(fwd, rA); pack(fwd, cA);
            packv(fwd, dataA.data(), sizeA);
            pack(fwd, rB); pack(fwd, cB);
            packv(fwd, dataB.data(), sizeB);

            // C. Enviar al Worker
            sendMSG(targetSocket, fwd);

            // D. Esperar respuesta del Worker
            vector<unsigned char> response;
            recvMSG(targetSocket, response);

            // E. Enviar respuesta al Cliente
            sendMSG(clientId, response);

            // F. Liberar Worker
            mtxWorkers.lock();
            workers[targetId].busy = false;
            mtxWorkers.unlock();
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