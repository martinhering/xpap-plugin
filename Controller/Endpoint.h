//
//  Endpoint.h
//  XPAP
//
//  Created by Martin Hering on 08.09.13.
//
//

#ifndef __XPAP__Endpoint__
#define __XPAP__Endpoint__

#include "UDPConnection.h"


namespace XPAP {
    
    class Endpoint
    {
    public:
        explicit Endpoint(const char* dataRefName, int identifier, int type);
        virtual ~Endpoint();
        
        const int& getType() {
            return m_type;
        }
        
        const int& getIdentifier() {
            return m_identifier;
        }
        
        void sendValue(UDPConnection& connection);
        void sendValue(UDPConnection& connection, bool initial);
        
    private:
        int             m_identifier;
        int             m_type;
        
        XPLMDataRef     m_dataRef;
        int             m_integerValue;
        float           m_floatValue;
    };
    
    typedef std::vector<Endpoint>
    EndpointList;
    
    typedef EndpointList::iterator
    EndpointListIterator;
}

#endif /* defined(__XPAP__Endpoint__) */
