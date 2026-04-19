cmake_minimum_required(VERSION 3.10)
project(MathCluster LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- SERVER (WORKER) ---
# Compila con Math.cpp (Local) y GestorCalculo.cpp
add_executable(server
    server.cpp
    GestorCalculo.cpp
    Math.cpp
    utils.cpp
    utils.h
    structures.h
    Math.h
    GestorCalculo.h
)
target_link_libraries(server pthread)

# --- CLIENT ---
# Compila con MathRemota.cpp (Remoto/Proxy)
add_executable(client
    main_math.cpp
    MathRemota.cpp
    utils.cpp
    utils.h
    structures.h
    Math.h
)
target_link_libraries(client pthread)

# --- BROKER ---
add_executable(broker
    broker.cpp
    utils.cpp
    utils.h
    structures.h
)
target_link_libraries(broker pthread)