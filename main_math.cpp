#include <iostream>
#include <map>
#include <string>
#include "Math.h" // Usará MathRemota.cpp al compilar
#include "structures.h"

string brokerIp;
int brokerPort;

int main(int argc, char** argv) {
    if (argc != 3) {
        cout << "Usage: client <BROKER_IP> <BROKER_PORT>" << endl;
        return 0;
    }
    brokerIp = argv[1];
    brokerPort = atoi(argv[2]);

    Math mm;
    map<string, matrix_t> memory;
    string cmd;

    cout << "--- Math Cluster Client ---" << endl;

    while (true) {
        cout << "> ";
        cin >> cmd;

        if (cmd == "exit") break;
        else if (cmd == "load") {
            string file, var; cin >> file >> var;
            memory[var] = mm.loadMatrix(file);
            cout << "Cargada " << var << endl;
        }
        else if (cmd == "save") {
            string var, file; cin >> var >> file;
            if(memory.count(var)) mm.saveMatrix(file, memory[var]);
        }
        else if (cmd == "mult") {
            string vA, vB, vC; cin >> vA >> vB >> vC;
            if(memory.count(vA) && memory.count(vB)) {
                cout << "Enviando..." << endl;
                memory[vC] = mm.multiply(memory[vA], memory[vB]);
                cout << "Resultado en " << vC << endl;
            }
        }
        else if (cmd == "log") {
            mm.downloadLog();
        }
    }
    return 0;
}