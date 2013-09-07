//
//  B777MCP.cpp
//  XPAP
//
//  Created by Martin Hering on 07.09.13.
//
//




#include "B777MCP.h"


using namespace XPAP;

B777MCP::B777MCP(IPAddress ip) : m_ip(ip) {
    m_altitude = XPLMFindDataRef("sim/cockpit/autopilot/altitude");
}

B777MCP::~B777MCP() {
    
}

void B777MCP::loop() {
    
    float mcp_altitude = XPLMGetDataf(m_altitude);
    
    
}