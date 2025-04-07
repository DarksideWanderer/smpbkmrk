#include "dsocket.h"
#include "dtool/dlogger.h"
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#endif

namespace dag {
namespace sockett {
	using namespace dag::tool;

	// 构造函数
	Socket::Socket() {
		m_ip = "";
		m_port = 0;
		m_sockfd = 0;

#ifdef _WIN32
		// 初始化 Winsock
		WSADATA wsaData;
		int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != 0) {
			logError("WSAStartup failed with error: ", iResult);
			return;
		}
#endif

		// 创建套接字
		m_sockfd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (m_sockfd < 0) {
			logError("create socket error: errno=", errno, " errmsg=", strerror(errno));
		}
		logDebug("create socket success!");
	}

	// 析构函数
	Socket::~Socket() {
		close();
#ifdef _WIN32
		// 清理 Winsock
		WSACleanup();
#endif
	}

	// 绑定套接字
	bool Socket::bind(const std::string &ip, int port) {
		sockaddr_in sockaddrs;
		memset(&sockaddrs, 0, sizeof(sockaddrs));
		sockaddrs.sin_family = AF_INET;
		if (ip.empty())
			sockaddrs.sin_addr.s_addr = htonl(INADDR_ANY);
		else
			sockaddrs.sin_addr.s_addr = inet_addr(ip.c_str());
		sockaddrs.sin_port = htons(port);

		if (::bind(m_sockfd, (sockaddr *)&sockaddrs, sizeof(sockaddrs)) < 0) {
			logError("socket bind error: errno=", errno, " errmsg=", strerror(errno));
			return false;
		}

		m_ip = ip;
		m_port = port;
		logDebug("socket bind success: ip=", ip, " port=", port);
		return true;
	}

	// 开始监听
	bool Socket::listen(int backlog) {
		if (::listen(m_sockfd, backlog) < 0) {
			logError("socket listen error: errno=", errno, " errmsg=", strerror(errno));
			return false;
		}
		logDebug("socket listening ...");
		return true;
	}

	// 连接到远程服务器
	bool Socket::connect(const std::string &ip, int port) {
		sockaddr_in sockaddrs;
		memset(&sockaddrs, 0, sizeof(sockaddrs));
		sockaddrs.sin_family = AF_INET;
		sockaddrs.sin_addr.s_addr = inet_addr(ip.c_str());
		sockaddrs.sin_port = htons(port);

		if (::connect(m_sockfd, (sockaddr *)&sockaddrs, sizeof(sockaddrs)) < 0) {
			logError("socket connect error: errno=", errno, " errmsg=", strerror(errno));
			return false;
		}

		m_ip = ip;
		m_port = port;
		logDebug("socket connect success!");
		return true;
	}

	// 接受连接
	int Socket::accept() {
		int connfd = ::accept(m_sockfd, nullptr, nullptr);
		if (connfd < 0) {
			logError("socket accept error: errno=", errno, " errmsg=", strerror(errno));
			return -1;
		}
		logDebug("socket accept: conn=", connfd);
		return connfd;
	}

	// 发送数据
	int Socket::send(const char *buf, int len, int connfd) {
		return ::send(connfd == -1 ? m_sockfd : connfd, buf, len, 0);
	}

	// 接收数据
	int Socket::recv(char *buf, int len, int connfd) {
		return ::recv(connfd == -1 ? m_sockfd : connfd, buf, len, 0);
	}

	// 关闭套接字
	void Socket::close() {
		if (m_sockfd > 0) {
#ifdef _WIN32
			::closesocket(m_sockfd);
#else
			::close(m_sockfd);
#endif
			m_sockfd = 0;
		}
	}

	// 设置为非阻塞模式
	bool Socket::setNonBlocking() {
		#ifdef _WIN32
			// Windows 系统下设置非阻塞模式
			u_long mode = 1;  // 1 表示非阻塞模式
			if (ioctlsocket(m_sockfd, FIONBIO, &mode) != 0) {
				logError("socket setNonBlocking error: errno=", errno, " errmsg=", strerror(errno));
				return false;
			}
		#else
			// 类 Unix 系统下设置非阻塞模式
			int flags = fcntl(m_sockfd, F_GETFL, 0);
			if (flags < 0) {
				logError("socket setNonBlocking error errno=", errno, " errmsg=", strerror(errno));
				return false;
			}
			flags |= O_NONBLOCK;
			if (fcntl(m_sockfd, F_SETFL, flags) < 0) {
				logError("socket setNonBlocking error errno=", errno, " errmsg=", strerror(errno));
				return false;
			}
		#endif
			return true;
		}

		bool Socket::setSentBuffer(int size) {
			int result = setsockopt(m_sockfd, SOL_SOCKET, SO_RCVBUF, (const char*)&size, sizeof(size));
			if (result < 0) {
				logError("socket setSentBuffer error errno=", errno, " errmsg=", strerror(errno));
				return false;
			}
			return true;
		}
		
		// 设置接收缓冲区大小
		bool Socket::setRecvBuffer(int size) {
			int result = setsockopt(m_sockfd, SOL_SOCKET, SO_RCVBUF, (const char*)&size, sizeof(size));
			if (result < 0) {
				logError("socket setRecvBuffer error errno=", errno, " errmsg=", strerror(errno));
				return false;
			}
			return true;
		}
}
}
