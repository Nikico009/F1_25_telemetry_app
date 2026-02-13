#!/usr/bin/env python3

import socket
import struct
import time
import math
import random

# ================= CONFIG =================
TARGET_IP = "127.0.0.1"
TARGET_PORT = 20777
FREQUENCY = 60  # Hz

# Packet format:
# timestamp (uint32)
# rpm       (uint16)
# speed     (float)
# throttle  (float)
# brake     (float)
PACKET_FORMAT = "!I H f f f"

# ==========================================

def generate_telemetry(start_time):
    t = time.perf_counter() - start_time

    # Simulación dinámica
    rpm = int(9000 + 2500 * math.sin(t * 2))
    speed = 250 + 80 * math.sin(t * 0.7)
    throttle = (math.sin(t * 1.5) + 1) / 2
    brake = max(0.0, math.sin(t * 0.3 - 1.0))

    # Clamp seguridad
    rpm = max(0, min(15000, rpm))
    throttle = max(0.0, min(1.0, throttle))
    brake = max(0.0, min(1.0, brake))

    timestamp = int(t * 1000)  # ms

    return struct.pack(
        PACKET_FORMAT,
        timestamp,
        rpm,
        speed,
        throttle,
        brake
    )

def main():
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    addr = (TARGET_IP, TARGET_PORT)

    print(f"UDP Telemetry Server running at {TARGET_IP}:{TARGET_PORT}")
    print(f"Sending at {FREQUENCY} Hz...\n")

    start_time = time.perf_counter()
    next_tick = time.perf_counter()

    try:
        while True:
            next_tick += 1.0 / FREQUENCY

            packet = generate_telemetry(start_time)
            sock.sendto(packet, addr)
            print(packet)

            sleep_time = next_tick - time.perf_counter()
            if sleep_time > 0:
                time.sleep(sleep_time)

    except KeyboardInterrupt:
        print("\nServer stopped.")
    finally:
        sock.close()

if __name__ == "__main__":
    main()

