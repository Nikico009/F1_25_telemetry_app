import socket
import struct

HOST = "0.0.0.0"
PORT = 8080

HEADER_FORMAT = "!IH"   # uint32 id, uint16 len
HEADER_SIZE = struct.calcsize(HEADER_FORMAT)


def recv_exact(conn, size):
    data = b""
    while len(data) < size:
        chunk = conn.recv(size - len(data))
        if not chunk:
            return None
        data += chunk
    return data


def main():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind((HOST, PORT))
        s.listen()

        print(f"Listening on {HOST}:{PORT}")

        conn, addr = s.accept()
        print("Connected from", addr)

        with conn:
            while True:
                # 1️⃣ Recibir header
                header = recv_exact(conn, HEADER_SIZE)
                if header is None:
                    print("Connection closed")
                    break

                packet_id, length = struct.unpack(HEADER_FORMAT, header)

                # 2️⃣ Recibir payload
                payload = recv_exact(conn, length)
                if payload is None:
                    print("Connection closed while receiving payload")
                    break

                print(f"Packet id={packet_id}, len={length}, payload={payload}")


if __name__ == "__main__":
    main()

