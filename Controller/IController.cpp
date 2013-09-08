//
//  IController.cpp
//  XPAP
//
//  Created by Martin Hering on 08.09.13.
//
//

#include "XPAPProtocol.h"
#include "IController.h"

using namespace XPAP;

IController::IController(IPAddress ip) : m_ip(ip) {
    
    m_connection = UDPConnection();
    m_connection.bind(24555);
    m_connection.setRemoteAddress(m_ip, 24555);
}

IController::~IController() {
    
}

void IController::initialize() {
}

void IController::loop() {
}
