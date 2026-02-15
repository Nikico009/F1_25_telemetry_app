#ifndef TCP_EXPORTER_H
#define TCP_EXPORTER_H

#include "packet.h"

int serialize_packet(const packet_t *pkt, uint8_t *buffer);

int tcp_export(const packet_t *packet);

#endif
