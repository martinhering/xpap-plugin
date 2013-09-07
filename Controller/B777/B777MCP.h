//
//  B777MCP.h
//  XPAP
//
//  Created by Martin Hering on 07.09.13.
//
//

#ifndef __XPAP__B777MCP__
#define __XPAP__B777MCP__

#include "IPAddress.h"
#include <iostream>

namespace XPAP {
    
    class B777MCP {
        
    public:
        
        explicit B777MCP(IPAddress ip);
        virtual ~B777MCP();
        
        
        void loop();
        
    private:

        IPAddress m_ip;

        XPLMDataRef m_altitude;
    };

};



#endif /* defined(__XPAP__B777MCP__) */
