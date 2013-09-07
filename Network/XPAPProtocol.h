//
//  XPAPProtocol.h
//  XPAP
//
//  Created by Martin Hering on 07.09.13.
//
//

#ifndef XPAP_XPAPProtocol_h
#define XPAP_XPAPProtocol_h


typedef struct {
    uint32_t            endpoint;
    uint8_t             payload_type;
    uint16_t            payload_size;
} XPAPUDPHeader_t;

#define XPAP_MAX_PACKET_SIZE 512

#endif
