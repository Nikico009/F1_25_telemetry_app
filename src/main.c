#include <stdio.h>
#include "network/udp_receiver.h"

#define PORT 20777
#define MAX_PCKT_LEN 512

int main()
{
    telemetry_packet_t packet;

    if (udp_init(PORT) != 0) {
        return 1;
    }

    printf("Listening on port %d...\n", PORT); // Only for debbuging

    while (1) {
        if (udp_receive(&packet) == 0) {
            printf("t=%u ms | rpm=%u | speed=%.1f | throttle=%.2f | brake=%.2f\n", packet.timestamp, packet.rpm, packet.speed, packet.throttle, packet.brake);

            uint8_t buffer[MAX_PCKT_LEN] = {0};
            int len = serialize_packet(&packet, buffer);
            
            if(len <= 0){
              fprintf(stderr, "Serializing error\n");
            }
            if(tcp_export(buffer, len)){
              fprintf(stderr, "TCP export error\n");
            }
        }
    }

    return 0;
}

