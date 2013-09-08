//
//  B777MCP.h
//  XPAP
//
//  Created by Martin Hering on 07.09.13.
//
//

#ifndef __XPAP__B777MCP__
#define __XPAP__B777MCP__

#include "IController.h"
#include "Endpoint.h"

namespace XPAP {
    
    class B777MCP : public IController
    {
        
    public:
        
        explicit B777MCP(IPAddress ip);
        virtual ~B777MCP();
        
        virtual void initialize();
        virtual void loop();
        
    private:
        
        EndpointList    m_endpoints;
    };

};



#endif /* defined(__XPAP__B777MCP__) */
