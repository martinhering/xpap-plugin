/*
 *  UDPConnection.h
 *
 *  Created by Martin Hering on 02.09.10.
 *  Copyright 2010 Vemedio. All rights reserved.
 *
 */

#ifndef UDPConnection_
#define UDPConnection_

#include <sys/types.h>
#include <netinet/in.h>

#pragma GCC visibility push(default)

namespace XPAP
{
	class IPAddress;
	
    class UDPConnection
    {
    public:
		explicit UDPConnection();
		explicit UDPConnection(int type);
        virtual ~UDPConnection() { };
        
		bool bind(in_port_t client_port, const std::string& interface = "");
		bool setRemoteAddress(const IPAddress& address, in_port_t server_port);
		
        void disconnect() const;
		
        // communication
        bool sendData(const void* data, const size_t data_size);
        int recvData(void* data, size_t& data_size);
		
		const IPAddress getLocalAddress();
		in_port_t getLocalPort() const;
		
		const IPAddress getRemoteAddress();
		in_port_t getRemotePort() const;
		
		bool& rBlocking() {
			return m_blocking;
		}
		
		int& rTimeout() {
			return m_timeout;
		}
		
		int getLastError();
		
    protected:
		int					m_af_type;
        struct sockaddr_in	m_server_addr4;
        struct sockaddr_in	m_client_addr4;
		struct sockaddr_in6	m_server_addr6;
        struct sockaddr_in6	m_client_addr6;
		struct sockaddr*	m_server_addr;
		struct sockaddr*	m_client_addr;
        int                 m_socket;
		
		bool				m_blocking;
		int					m_timeout;
		
        size_t              m_data_written;
        size_t              m_data_read;
    };
}

#pragma GCC visibility pop

#endif