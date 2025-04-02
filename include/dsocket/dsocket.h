#pragma once

#include<bits/stdc++.h>

namespace dag{
namespace sockett{
	class Socket{
	public:
		Socket();
		~Socket();
		bool bind(const std::string&ip,int port);
		bool listen(int backlog);
		bool connect(const std::string&ip,int port);
		int accept();
		int send(const char * buf,int len,int connfd=-1);
		int recv(char * buf,int len,int connfd=-1);
		void close();
		
		bool setNonBlocking();
		bool setSentBuffer(int size);
		bool setRecvBuffer(int size);
	protected:
		std::string m_ip;
		int m_port;
		int m_sockfd;
	};
}
}