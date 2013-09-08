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

B777MCP::B777MCP(IPAddress ip) : IController(ip)
{
    m_endpoints.push_back(Endpoint("sim/cockpit/autopilot/autopilot_mode", XPAPEndpointAutopilotMode, XPAPPayloadTypeInteger));
    m_endpoints.push_back(Endpoint("sim/cockpit/autopilot/altitude", XPAPEndpointAutopilotDialedAltitude, XPAPPayloadTypeFraction));
}

B777MCP::~B777MCP() {
    
}

void B777MCP::initialize()
{
    CDebugLog("initialize");
    
    EndpointListIterator iter;
    for(iter = m_endpoints.begin(); iter != m_endpoints.end(); iter++)
	{
		Endpoint endpoint = *iter;
        endpoint.sendValue(m_connection, true);
	}
}


void B777MCP::loop()
{
    EndpointListIterator iter;
    for(iter = m_endpoints.begin(); iter != m_endpoints.end(); iter++)
	{
		Endpoint endpoint = *iter;
        endpoint.sendValue(m_connection);
	}
}