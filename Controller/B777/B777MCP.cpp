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
    m_altitude = XPLMFindDataRef("sim/cockpit/autopilot/altitude");
    
    m_connection = UDPConnection();
    m_connection.bind(24555);
    m_connection.setRemoteAddress(m_ip, 24555);
}

B777MCP::~B777MCP() {
    
}

void B777MCP::loop() {
    
    float mcp_altitude = XPLMGetDataf(m_altitude);
    
    uint32_t payload = htonl((uint32_t)mcp_altitude);
    
    XPAPUDPHeader_t header;
    header.endpoint = htonl(1);
    header.payload_type = htonl(1);
    header.payload_size = htonl(4);
    
    uint8_t packet[XPAP_MAX_PACKET_SIZE];
    memcpy(packet, &header, sizeof(header));
    memcpy(packet+sizeof(header), &payload, header.payload_size);
    
    m_connection.sendData(&packet, sizeof(header)+header.payload_size);
}