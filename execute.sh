#!/bin/bash
set -e

echo "=== Building C project ==="

# Create build dir
if [ ! -d "build" ]; then
  cmake -S . -B build
fi

cmake --build build

echo "=== Starting backend ==="
python3 backend/main.py &
BACKEND_PID=$!

# Hold for server to be ready
sleep 1

echo "=== Starting bridge ==="
./build/telemetry_bridge

kill $BACKEND_PID
