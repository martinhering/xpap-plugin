/*
 *  VDUDPConnection.cpp
 *  VDFoundation
 *
 *  Created by Martin Hering on 02.09.10.
 *  Copyright 2010 Vemedio. All rights reserved.
 *
 */

#include <time.h>
#include <iostream>
#include <sstream>
#include <fcntl.h>
#include <errno.h>
#include <math.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <net/if.h>

#include "IPAddress.h"
#include "UDPConnection.h"

extern int errno;

using namespace XPAP;

UDPConnection::UDPConnection()
{
	m_blocking = true;
	m_timeout = 2;
	
	m_data_written = 0;
    m_data_read = 0;
	m_af_type = AF_INET;
}

UDPConnection::UDPConnection(int type)
{
	m_blocking = true;
	m_timeout = 2;
	
	m_data_written = 0;
    m_data_read = 0;
	m_af_type = type;
}

bool UDPConnection::bind(in_port_t client_port, const std::string& interface)
{
	int ret, set, sock_size;
	
	if (client_port == 0) {
		client_port = (in_port_t)(((double)rand() / RAND_MAX) * 16383 + 49152);
	}
	
	if (m_af_type == AF_INET) {
		m_client_addr4.sin_family = AF_INET;
		m_client_addr4.sin_addr.s_addr = INADDR_ANY;
		m_client_addr4.sin_port = htons(client_port);
		m_client_addr = (struct sockaddr*)&m_client_addr4;
	}
	else if (m_af_type == AF_INET6)
	{	
		m_client_addr6.sin6_family = AF_INET6;
		m_client_addr6.sin6_addr = in6addr_any;
		m_client_addr6.sin6_port = htons(client_port);
		if (interface.size() > 0) {
			m_client_addr6.sin6_scope_id = if_nametoindex(interface.c_str());
		}
		m_client_addr = (struct sockaddr*)&m_client_addr6;
	}
	
	sock_size = (m_af_type == AF_INET6) ? sizeof(struct sockaddr_in6) : sizeof(struct sockaddr_in);
    
	int type = (m_af_type == AF_INET6) ? PF_INET6 : PF_INET;
    m_socket = ::socket(type, SOCK_DGRAM, IPPROTO_UDP);
    if (m_socket < 0) {
        return false;
    }

	set = 1;
    setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &set, sizeof(int));
	
	set = 1;
	setsockopt(m_socket, SOL_SOCKET, SO_NOSIGPIPE, (void *)&set, sizeof(int));
	signal(SIGPIPE, SIG_IGN);
	
	if (!m_blocking)
	{
		// timeout
		struct timeval	tout;
		tout.tv_sec = m_timeout;
		tout.tv_usec = 0;
		
		setsockopt(m_socket, SOL_SOCKET, SO_RCVTIMEO, &tout, sizeof(tout));
		setsockopt(m_socket, SOL_SOCKET, SO_SNDTIMEO, &tout, sizeof(tout));
	}

	int size = sock_size;
	ret = ::bind( m_socket, m_client_addr, size);
	if(ret < 0)
	{
		int valopt=0;
		socklen_t lon = sizeof(int); 
		getsockopt(m_socket, SOL_SOCKET, SO_ERROR, (void*)(&valopt), &lon); 
		CErrLog("bind failed() " << strerror(valopt)); 
		disconnect();
		return false;
	}
	
	return true;
}

bool UDPConnection::setRemoteAddress(const IPAddress& address, in_port_t server_port)
{
	std::string interface = address.getInterface();
	
	if (m_af_type == AF_INET) {
		m_server_addr4.sin_family = AF_INET;
		m_server_addr4.sin_port = htons(server_port);
		m_server_addr4.sin_addr = address.GetAddr();
		m_server_addr = (struct sockaddr*)&m_server_addr4;
	}
	else if (m_af_type == AF_INET6)
	{	
		m_server_addr6.sin6_family = AF_INET6;
		m_server_addr6.sin6_port = htons(server_port);
		m_server_addr6.sin6_addr = address.GetIPv6Addr();
		if (interface.size() > 0) {
			m_server_addr6.sin6_scope_id = if_nametoindex(interface.c_str());
		}
		m_server_addr = (struct sockaddr*)&m_server_addr6;
	}
	
	return true;
}

void UDPConnection::disconnect() const
{
	if (m_socket >= 0) {
		::close(m_socket);
	}
}

bool UDPConnection::sendData(const void* data, const size_t data_size)
{
	int ret = 0;
	size_t data_to_send = data_size;
	uint8_t* data_ptr = (uint8_t*)data;
    do
	{
		size_t size = (m_af_type == AF_INET6) ? sizeof(struct sockaddr_in6) : sizeof(struct sockaddr_in);
		size_t part_size = (data_to_send < 1400) ? data_to_send : 1400;
		
		ret = ::sendto(m_socket, data_ptr, part_size, 0, m_server_addr, size);
        if (ret < 0) {
            break;
        }
		m_data_written += ret;
		data_ptr += ret;
		data_to_send -= ret;
    } while (data_to_send > 0);
    
    return (ret>0);
}

int UDPConnection::recvData(void* data, size_t& data_size)
{
	socklen_t size = (m_af_type == AF_INET6) ? sizeof(struct sockaddr_in6) : sizeof(struct sockaddr_in);
    int ret = ::recvfrom(m_socket, data, data_size, 0, m_server_addr, &size);
    if (ret > 0) {
        data_size = ret;
        m_data_read += ret;
        return ret;
    }
    
    data_size = 0;
    return ret;
}

const IPAddress UDPConnection::getLocalAddress()
{
	if (m_af_type == AF_INET) {
		socklen_t length = sizeof(struct sockaddr_in);
		struct sockaddr_in addr;
		
		getsockname(m_socket, (sockaddr*)&addr, &length);
		IPAddress address(addr.sin_addr);
		return address;
	}
	else if (m_af_type == AF_INET6) {
		socklen_t length = sizeof(struct sockaddr_in6);
		struct sockaddr_in6 addr;
		
		getsockname(m_socket, (sockaddr*)&addr, &length);
		IPAddress address(addr.sin6_addr);
		return address;
	}
	return IPAddress();
}

in_port_t UDPConnection::getLocalPort() const
{
	if (m_af_type == AF_INET) {
		return ntohs(m_client_addr4.sin_port);
	} else if (m_af_type == AF_INET6) {
		return ntohs(m_client_addr6.sin6_port);
	}
	return 0;
}

const IPAddress UDPConnection::getRemoteAddress()
{
	if (m_af_type == AF_INET) {
		socklen_t length = sizeof(struct sockaddr_in);
		struct sockaddr_in addr;
		
		getpeername(m_socket, (sockaddr*)&addr, &length);
		IPAddress address(addr.sin_addr);
		return address;
	}
	else if (m_af_type == AF_INET6) {
		socklen_t length = sizeof(struct sockaddr_in6);
		struct sockaddr_in6 addr;
		
		getpeername(m_socket, (sockaddr*)&addr, &length);
		IPAddress address(addr.sin6_addr);
		return address;
	}
	return IPAddress();
}

in_port_t UDPConnection::getRemotePort() const
{
	if (m_af_type == AF_INET) {
		return ntohs(m_server_addr4.sin_port);
	} else if (m_af_type == AF_INET6) {
		return ntohs(m_server_addr6.sin6_port);
	}
	return 0;
}

int UDPConnection::getLastError()
{
	return errno;	
}
