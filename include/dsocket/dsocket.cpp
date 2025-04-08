// #include"dsocket.h"
// #include"dtool/dlogger.h"
// #include<sys/socket.h>
// #include<netinet/in.h>
// #include<unistd.h>
// #include<arpa/inet.h>
// #include<fcntl.h>

// namespace dag{
// namespace sockett{
// 	using namespace dag::tool;
// 	Socket::Socket(){
// 		m_ip="";m_port=0;m_sockfd=0;
		
// 		m_sockfd=::socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
// 		if(m_sockfd<0){
// 			logError("create socket error: errno=",errno," errmsg=",strerror(errno));
// 		}
// 		logDebug("create socket success!");
// 	}
// 	Socket::~Socket(){
// 		close();
// 	}
// 	bool Socket::bind(const std::string&ip,int port){
// 		sockaddr_in sockaddrs;
// 		memset(&sockaddrs,0,sizeof(sockaddrs));
// 		sockaddrs.sin_family=AF_INET;
// 		if(ip.empty())
// 			sockaddrs.sin_addr.s_addr=htonl(INADDR_ANY);
// 		else
// 			sockaddrs.sin_addr.s_addr=inet_addr(ip.c_str());
// 		sockaddrs.sin_port=htons(port);
// 		if(::bind(Socket::m_sockfd,(sockaddr*)&sockaddrs,sizeof(sockaddrs))<0){//Error
// 			logError("socket bind error: errno=",errno," errmsg=",strerror(errno));
// 			return false;
// 		}
// 		m_ip=ip;
// 		m_port=port;
// 		logDebug("socket bind success: ip=",ip," port=",port);
// 		return true;
// 	}
// 	bool Socket::listen(int backlog){
// 		if(::listen(m_sockfd,backlog)<0){
// 			logError("socket listen error: errno=",errno ," errmsg=",strerror(errno));
// 			return false;
// 		}
// 		logDebug("socket listening ...");
// 		return true;
// 	}
// 	bool Socket::connect(const std::string&ip,int port){
// 		sockaddr_in sockaddrs;
// 		memset(&sockaddrs,0,sizeof(sockaddrs));
// 		sockaddrs.sin_family=AF_INET;
// 		sockaddrs.sin_addr.s_addr=inet_addr(ip.c_str());
// 		sockaddrs.sin_port=htons(port);
		
// 		if(::connect(m_sockfd,(sockaddr*)&sockaddrs,sizeof(sockaddrs))<0){
// 			logError("socket connect error: errno=",errno ," errmsg=",strerror(errno));
// 			return false;
// 		}
// 		m_ip=ip;
// 		m_port=port;
// 		logDebug("socket connect success!");
// 		return true;
// 	}
// 	int Socket::accept(){
// 		int connfd=::accept(m_sockfd,nullptr,nullptr);
// 		if(connfd<0){
// 			logError("socket accept error: errno=",errno ," errmsg=",strerror(errno));
// 			return -1;
// 		}
// 		logDebug("socket accept: conn=",connfd);
// 		return connfd;
// 	}
// 	int Socket::send(const char * buf,int len,int connfd){
// 		return ::send(connfd==-1?m_sockfd:connfd,buf,len,0);
// 	}
// 	int Socket::recv(char *buf,int len,int connfd){
// 		return ::recv(connfd==-1?m_sockfd:connfd,buf,len,0);
// 	}
// 	void Socket::close(){
// 		if(m_sockfd>0){
// 			::close(m_sockfd);
// 			m_sockfd=0;
// 		}
// 	}
// 	bool Socket::setNonBlocking(){
// 		int flags=fcntl(m_sockfd,F_GETFL,0);
// 		if(flags<0){
// 			logError("socket setNonBlocking error errno=",errno ," errmsg=",strerror(errno));
// 			return false;
// 		}
// 		flags|=O_NONBLOCK;
// 		if(fcntl(m_sockfd,F_SETFL,flags)<0){
// 			logError("socket setNonBlocking error errno=",errno ," errmsg=",strerror(errno));
// 			return false;
// 		}
// 		return true;
// 	}
// 	bool Socket::setSentBuffer(int size){
// 		if(setsockopt(m_sockfd,SOL_SOCKET,SO_SNDBUF,&size,sizeof(size))<0){
// 			logError("socket setSentBuffer error errno=",errno ," errmsg=",strerror(errno));
// 			return false;
// 		}
// 		return true;
// 	}
// 	bool Socket::setRecvBuffer(int size){
// 		if(setsockopt(m_sockfd,SOL_SOCKET,SO_RCVBUF,&size,sizeof(size))<0){
// 			logError("socket setRecvBuffer error errno=",errno ," errmsg=",strerror(errno));
// 			return false;
// 		}
// 		return true;
// 	}
// }
// }
