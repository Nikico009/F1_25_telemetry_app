#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "tcp_exporter.h"

#define BACKEND_IP   "127.0.0.1"
#define BACKEND_PORT 6900

int serialize_packet(const packet_t *pkt, uint8_t *buffer)
{
    uint32_t id_net  = htonl(pkt->id);
    uint16_t len_net = htons(pkt->len);

    memcpy(buffer, &id_net, sizeof(uint32_t));
    memcpy(buffer + 4, &len_net, sizeof(uint16_t));
    memcpy(buffer + 6, pkt->payload, pkt->len);

    return 6 + pkt->len;
}

int tcp_export(const packet_t *packet)
{
    int sock;
    struct sockaddr_in server_addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(BACKEND_PORT);
    inet_pton(AF_INET, BACKEND_IP, &server_addr.sin_addr);

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) { // Connect to tcp_server
        perror("connect");
        close(sock);
        return -1;
    }

    if (send(sock, packet, sizeof(packet_t), 0) < 0) { // Send trhough tcp_server
        perror("send");
        close(sock);
        return -1;
    }

    close(sock);
    return 0;
}
