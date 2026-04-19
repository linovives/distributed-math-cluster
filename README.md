MathCluster — README

Descripción general
- Demo de multiplicación de matrices distribuida con tres roles: `broker`, `worker`, `client`.

Compilación
```bash
mkdir -p build
cd build
cmake ..
make -j
```

Ejecución manual (flujo de desarrollo)
- Inicia el broker (puerto por defecto 9000):
```bash
./broker 9000
```
- Inicia uno o más workers (cada uno se registra con el broker):
```bash
./worker 127.0.0.1 9000
```
- Inicia el cliente (CLI interactivo):
```bash
./client 127.0.0.1 9000
```
- Ejemplo de comandos del cliente dentro de la CLI:
```
load ../sampleA.txt A
load ../sampleB.txt B
mult A B C
save C ../out.txt
exit
```

Prueba automatizada
- Un script helper `run_test.sh` compila el proyecto, inicia broker + 2 workers, ejecuta el cliente con los comandos anteriores, escribe logs en `logs/` y produce `out.txt`.

Ejecutar la prueba
```bash
chmod +x run_test.sh
./run_test.sh
```

Resultado esperado
- `out.txt` debe contener la matriz multiplicada (para los archivos de ejemplo incluidos):
```
2 2
58 64
139 154
```

Notas y solución de problemas
- El proyecto depende de `utils.h` / `utils.cpp` para helpers de socket y funciones `pack`/`unpack`; estos archivos están incluidos en el repositorio.
- El broker usa threads desacoplados y una cola `waitingClients`; evita refactorizar el flujo de red sin preservar estos comportamientos.

Logs
- `run_test.sh` escribe logs de ejecución en `logs/broker.log`, `logs/worker1.log`, `logs/worker2.log`, `logs/client.log`.
