# Distributed Math Cluster
Matrix Multiplication · Broker/Worker Pattern · Fault Tolerance

![C++](https://img.shields.io/badge/C++-17-blue)
![CMake](https://img.shields.io/badge/Build-CMake-red)
![Sockets](https://img.shields.io/badge/Network-POSIX%20Sockets-lightgrey)
![Linux](https://img.shields.io/badge/Platform-Linux%20%2F%20WSL-yellow)

## Overview

Distributed computing system in C++ that offloads CPU-intensive matrix multiplications
across multiple worker nodes, coordinated by a central broker.

Built as a practical implementation of the **Proxy / Skeleton** pattern studied in Distributed Systems,
where the client has no knowledge of the underlying network - it just calls `multiply()`.

```
[Client] ──── MSG_CALC_REQ ────► [Broker] ──── forwards ────► [Worker 1]
         ◄─── MSG_CALC_RES ────           ◄─── result ──────   [Worker 2]
                                                                [Worker N]
```

## Three Roles, One System

| Component | Role | Key file |
|-----------|------|----------|
| **Client** | CLI interface. Loads matrices, requests multiplications, shows results | `MathRemota.cpp` (Proxy) |
| **Broker** | Load balancer. Routes tasks, detects dead nodes, keeps event log | `broker.cpp` |
| **Worker** | Compute node. Unpacks matrices, multiplies, returns result | `GestorCalculo.cpp` (Skeleton) |

## Features

- **Transparent RPC** - client calls `multiply()` as if it were local; serialization happens under the hood
- **Load balancing** - broker picks the first available worker for each request
- **Keep Alive** - workers send a heartbeat every 2s; broker drops nodes silent for 10+ seconds
- **Remote logs** - client can pull the broker's full event history with the `log` command

## Requirements

- GCC with C++17 support
- CMake 3.10+
- Linux or WSL

## Build

```bash
mkdir -p build && cd build
cmake ..
make -j
```

Produces three executables in `build/`: `broker`, `server`, `client`.

## Usage

**1. Start the broker:**
```bash
./build/broker 9000
```

**2. Start one or more workers:**
```bash
./build/server 127.0.0.1 9000
```

**3. Start the client:**
```bash
./build/client 127.0.0.1 9000
```

### Client commands

| Command | Description |
|---------|-------------|
| `load <file> <var>` | Load a matrix from disk into memory |
| `save <var> <file>` | Save a matrix from memory to disk |
| `mult <A> <B> <C>` | Multiply A × B and store result in C |
| `log` | Fetch and print the broker's event log |
| `exit` | Close the client |

## Automated Test

```bash
chmod +x run_test.sh
./run_test.sh
```

Builds the project, starts broker + 2 workers, runs the client and checks the result.
Logs are written to `logs/`.

### Expected output

For the included sample matrices (`sampleA.txt` × `sampleB.txt`):

```
2 2
58 64
139 154
```

## Project Structure

```
├── Math.h / Math.cpp           # Local multiplication logic
├── MathRemota.cpp              # Client-side Proxy (serialization + RPC)
├── GestorCalculo.h / .cpp      # Worker-side Skeleton (deserialization + compute)
├── broker.cpp                  # Central router with load balancing and Keep Alive
├── server.cpp                  # Worker process
├── main_math.cpp               # Client CLI
├── utils.h / utils.cpp         # Socket helpers, pack / unpack
├── structures.h                # Shared types (matrix_t, MathMsgTypes)
├── sampleA.txt / sampleB.txt   # Example matrices
└── run_test.sh                 # End-to-end automated test
```

