#ifndef UDP_RECEIVER_H
#define UDP_RECEIVER_H

#include <stdint.h>

typedef struct {
    uint32_t timestamp;
    uint16_t rpm;
    float speed;
    float throttle;
    float brake;
} telemetry_packet_t;

int udp_init(int port);
int udp_receive(telemetry_packet_t *packet);

#endif

