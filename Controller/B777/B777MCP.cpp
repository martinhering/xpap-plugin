//
//  B777MCP.cpp
//  XPAP
//
//  Created by Martin Hering on 07.09.13.
//
//

#include "XPAPProtocol.h"
#include "B777MCP.h"


using namespace XPAP;

B777MCP::B777MCP(IPAddress ip) : m_ip(ip) {
    m_altitude_ref = XPLMFindDataRef("sim/cockpit/autopilot/altitude");
    
    m_connection = UDPConnection();
    m_connection.bind(24555);
    m_connection.setRemoteAddress(m_ip, 24555);
}

B777MCP::~B777MCP() {
    
}

void B777MCP::loop() {
    
    float altitude = XPLMGetDataf(m_altitude_ref);
    
    if ((uint32_t)altitude != m_altitude)
    {
        m_altitude = (uint32_t)altitude ;
        
        uint32_t payload = htonl((uint32_t)m_altitude);
        
        XPAPUDPHeader_t header;
        header.endpoint = htonl(1);
        header.payload_type = htons(1);
        header.payload_size = htons(sizeof(payload));
        
        uint8_t packet[XPAP_MAX_PACKET_SIZE];
        memcpy(packet, &header, sizeof(XPAPUDPHeader_t));
        memcpy(packet+sizeof(header), &payload, header.payload_size);
        
        m_connection.sendData(&packet, sizeof(XPAPUDPHeader_t)+sizeof(payload));
    }
    
    
}