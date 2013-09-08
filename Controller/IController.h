//
//  IController.h
//  XPAP
//
//  Created by Martin Hering on 08.09.13.
//
//

#ifndef __XPAP__IController__
#define __XPAP__IController__

#include "IPAddress.h"
#include "UDPConnection.h"

namespace XPAP
{
    class IController
    {
    public:
        
        explicit IController(IPAddress ip);
        virtual ~IController();
    
        virtual void initialize();
        virtual void loop();
    
    protected:
    
        IPAddress       m_ip;
        UDPConnection   m_connection;
    };
}



#endif /* defined(__XPAP__IController__) */
