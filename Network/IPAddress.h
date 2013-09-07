/*
 *  IPAddress.h
 *
 *  Created by Martin Hering on 06.03.09.
 *  Copyright 2009 vemedio. All rights reserved.
 *
 */

#include <string>
#include <arpa/inet.h>

#ifndef IPAddress_
#define IPAddress_

#pragma GCC visibility push(default)

namespace XPAP
{
    class IPAddress
    {
	public:
		explicit IPAddress();
		explicit IPAddress(int type);
		explicit IPAddress(struct in_addr adr);
		explicit IPAddress(struct in6_addr adr);
		explicit IPAddress(const std::string& host, int type = AF_INET);
		virtual ~IPAddress();
		
		bool isValid() const {
			return m_is_valid;
		};
		
		const struct in_addr GetAddr() const {
			return m_adr4;
		}
		
		const struct in6_addr GetIPv6Addr() const {
			return m_adr6;
		}
		
		int getAFType() const {
			return m_type;
		}
		
		const std::string getPrintable() const;
		
		std::string& rInterface() {
			return m_interface;
		}
		
		const std::string& getInterface() const {
			return m_interface;
		}
		
	private:
		bool m_is_valid;
		int m_type;
		struct in_addr m_adr4;
		struct in6_addr m_adr6;
		std::string m_interface;
    };
}

#pragma GCC visibility pop

#endif
