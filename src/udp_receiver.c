#include "udp_receiver.h"

#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

static int sockfd;
static struct sockaddr_in server_addr;

int udp_init(int port)
{
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket failed");
        return -1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        return -1;
    }

    return 0;
}

int udp_receive(telemetry_packet_t *packet)
{
    uint8_t buffer[18];
    ssize_t len = recv(sockfd, buffer, sizeof(buffer), 0);

    if (len != 18)
        return -1;

    uint32_t temp32;
    uint16_t temp16;

    // timestamp
    memcpy(&temp32, buffer + 0, 4);
    packet->timestamp = ntohl(temp32);

    // rpm
    memcpy(&temp16, buffer + 4, 2);
    packet->rpm = ntohs(temp16);

    // speed
    memcpy(&temp32, buffer + 6, 4);
    temp32 = ntohl(temp32);
    memcpy(&packet->speed, &temp32, 4);

    // throttle
    memcpy(&temp32, buffer + 10, 4);
    temp32 = ntohl(temp32);
    memcpy(&packet->throttle, &temp32, 4);

    // brake
    memcpy(&temp32, buffer + 14, 4);
    temp32 = ntohl(temp32);
    memcpy(&packet->brake, &temp32, 4);

    return 0;
}

