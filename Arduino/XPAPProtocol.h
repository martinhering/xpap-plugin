
#ifndef XPAPProtocol_
#define XPAPProtocol_

#pragma GCC visibility push(default)

#define XPAP_MAX_PACKET_SIZE 512
#define XPAP_PAYLOAD_SIZE_INTEGER (sizeof(uint32_t)*2)
#define XPAP_PAYLOAD_SIZE_FRACTION (sizeof(uint32_t)*2)

enum {
    XPAPEndpointAutopilotMode     = 1,
    XPAPEndpointAutopilotDialedAltitude,
};


enum {
    XPAPPayloadTypeInteger  = 1,
    XPAPPayloadTypeFraction = 2,
};


typedef struct {
    uint32_t            endpoint;
    uint16_t            payload_type;
    uint16_t            payload_size;
    union {
      struct {
        int32_t value;
        int32_t reserved;
      } integer;
      struct {
        int32_t numerator;
        int32_t denominator;
      } fraction;
    } payload;
} XPAPPacket_t;

#endif