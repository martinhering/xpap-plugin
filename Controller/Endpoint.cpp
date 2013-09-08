//
//  Endpoint.cpp
//  XPAP
//
//  Created by Martin Hering on 08.09.13.
//
//

#include "XPAPProtocol.h"
#include "Endpoint.h"

using namespace XPAP;

Endpoint::Endpoint(const char* dataRefName, int identifier, int type) : m_identifier(identifier), m_type(type)
{
    m_dataRef = XPLMFindDataRef(dataRefName);
}

Endpoint::~Endpoint()
{
    
}

void Endpoint::sendValue(UDPConnection& connection)
{
    this->sendValue(connection, false);
}

void Endpoint::sendValue(UDPConnection& connection, bool initial)
{
    switch (m_type)
    {
        case XPAPPayloadTypeInteger:
        {
            int value = XPLMGetDatai(m_dataRef);
            
            if (m_integerValue != value || initial)
            {
                m_integerValue = value;
             
                XPAPPacket_t packet;
                packet.endpoint = htonl(m_identifier);
                packet.payload_type = htons(m_type);
                packet.payload_size = htons(XPAP_PAYLOAD_SIZE_INTEGER);
                packet.payload.integer.value = htonl(value);
                
                connection.sendData(&packet, sizeof(XPAPPacket_t));
            }
            
            break;
        }
        case XPAPPayloadTypeFraction:
        {
            float value = XPLMGetDataf(m_dataRef);
            
            if (fabsf(m_floatValue - value) > 0.001 || initial)
            {
                m_floatValue = value;

                XPAPPacket_t packet;
                packet.endpoint = htonl(m_identifier);
                packet.payload_type = htons(m_type);
                packet.payload_size = htons(XPAP_PAYLOAD_SIZE_FRACTION);
                packet.payload.fraction.numerator = htonl((int)(value*1000));
                packet.payload.fraction.denominator = htonl(1000);
                
                connection.sendData(&packet, sizeof(XPAPPacket_t));
            }
        }
        default:
            break;
    }
}