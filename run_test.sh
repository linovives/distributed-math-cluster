#!/usr/bin/env bash
set -euo pipefail

# Simple end-to-end smoke test for MathCluster
# - builds the project
# - starts broker on port 9000
# - starts two workers
# - runs the client CLI with automated commands to multiply sampleA * sampleB

ROOT_DIR="$(cd "$(dirname "$0")" && pwd)"
BUILD_DIR="$ROOT_DIR/build"
LOG_DIR="$ROOT_DIR/logs"

mkdir -p "$BUILD_DIR"
mkdir -p "$LOG_DIR"

echo "Building..."
cd "$BUILD_DIR"
cmake .. > /dev/null
make -j 2

echo "Starting broker..."
./broker > "$LOG_DIR/broker.log" 2>&1 &
BROKER_PID=$!
sleep 1

echo "Starting worker 1..."
./worker 127.0.0.1 9000 > "$LOG_DIR/worker1.log" 2>&1 &
W1=$!
echo "Starting worker 2..."
./worker 127.0.0.1 9000 > "$LOG_DIR/worker2.log" 2>&1 &
W2=$!

sleep 1

echo "Running client CLI commands..."
./client 127.0.0.1 9000 <<'EOF' > "$LOG_DIR/client.log" 2>&1
load ../sampleA.txt A
load ../sampleB.txt B
mult A B C
save C ../out.txt
exit
EOF

sleep 1

echo "=== Client output ==="
cat "$LOG_DIR/client.log" || true

echo "=== Result out.txt ==="
if [ -f "$ROOT_DIR/out.txt" ]; then
  cat "$ROOT_DIR/out.txt"
else
  echo "out.txt not found"
fi

echo "Stopping background processes..."
kill "$W1" "$W2" "$BROKER_PID" 2>/dev/null || true
sleep 1
echo "Done. Logs in $LOG_DIR"
