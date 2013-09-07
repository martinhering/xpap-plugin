/*
 *  VDIPAddress.cpp
 *  VDShoutcast++
 *
 *  Created by Martin Hering on 06.03.09.
 *  Copyright 2009 vemedio. All rights reserved.
 *
 */


#include "IPAddress.h"

#include <iostream>
#include <sstream>

#include <netdb.h>
#include <netinet/in.h>

using namespace XPAP;

IPAddress::IPAddress()
{
	m_type = AF_INET;
    m_adr4.s_addr = INADDR_ANY;
	m_adr6 = in6addr_any;
}

IPAddress::~IPAddress()
{
	
}

IPAddress::IPAddress(int type)
{
	m_type = type;
    m_adr4.s_addr = INADDR_ANY;
	m_adr6 = in6addr_any;
}

IPAddress::IPAddress(struct in_addr adr)
{
    m_adr4 = adr;
	m_type = AF_INET;
}

IPAddress::IPAddress(struct in6_addr adr)
{
	m_adr6 = adr;
	m_type = AF_INET6;
}


IPAddress::IPAddress(const std::string& host_str, int type)
{
	const char* address = host_str.c_str();
	
	if (type == AF_INET)
	{
		if (!address) {
			m_adr4.s_addr = INADDR_ANY;
		} 
		else if (inet_aton(address, &m_adr4) == 0)
		{
			struct hostent * host_info;
			
			/* Might be a DNS name, try te resolve it */
			host_info = gethostbyname2(address, AF_INET);
			
			if (!host_info || !host_info->h_addr || host_info->h_addrtype != AF_INET || host_info->h_length != sizeof(m_adr4.s_addr)) {
				m_is_valid = false;
				CDebugLogW(L"Can not resolve " << host_str);
				return;
			}
			
			memcpy(&(m_adr4.s_addr), host_info->h_addr, host_info->h_length);
		}
	}
	else
	{
		if (!address) {
			m_adr6 = in6addr_any;
		}
		else if (inet_pton(AF_INET6, address, &m_adr6) == 0)
		{
			CErrLogW(L"Resolving IPv6 domains not supported");
		}
	}
	
	m_is_valid = true;
	m_type = type;
}

const std::string IPAddress::getPrintable() const
{
	if (m_type == AF_INET)
	{
		uint32_t a = ntohl(m_adr4.s_addr);
		
		std::ostringstream ip;
		ip << (a>>24) << "." << ((a>>16)&0xFF) << "." << ((a>>8)&0xFF) << "." << (a&0xFF);
		return ip.str();
	}
	else
	{
		char str[INET6_ADDRSTRLEN];
		inet_ntop(m_type, &m_adr6, str, INET6_ADDRSTRLEN);
		
        return std::string(str);
	}
	return "";
}
