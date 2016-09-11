/*
 * Connection.h
 *
 *  Created on: Sep 10, 2016
 *      Author: rrodgers
 */

#ifndef SRC_CONNECTION_H_
#define SRC_CONNECTION_H_
#include <netinet/in.h>
#include <sys/socket.h>
#include <netinet/ip6.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdint.h>

namespace BLS {
namespace AMS {
/**
 * \brief TCP connection management class.
 *
 * By default, we will be creating an IPv6 socket that will accept and communicate
 * with IPv4 connections mapped to IPv6. This offers greater flexibility without additional
 * complexity. (Assuming the server supports IPv6.) The purpose of this class is to streamline
 * handling of a received TCP connection.
 */
class Connection {
private:
	static uint64_t cfgFlags;
	struct sockaddr_in6 cli;
	socklen_t cliLen;
	int32_t socket;
	void init(void);
	uint64_t flags;
	char address[47];
	char *hostname;
	unsigned char *buffer;
	uint64_t bufferedDataLen;
	uint64_t totalBytesReceived;
	uint64_t totalBytesSent;
public:
	/**
	 * \brief Default constructor.
	 *
	 * This is the default constructor; it probably should never be used as there are no functions
	 * to set the socket, or address structures outside of the more complex constructor.
	 */
	Connection();
	/**
	 * \brief The real constructor.
	 *
	 * @param CLI
	 * @param CLILEN
	 * @param SOCKET
	 */
	Connection(struct sockaddr_in6 CLI, socklen_t CLILEN, int32_t SOCKET);
	/**
	 * \brief The destructor
	 */
	virtual ~Connection();
	const uint64_t FLAG_IPV4_MAPPED = 0x00000001;
	/**
	 * \brief Sets a flag on this Connection object.
	 * @param FLAG
	 */
	void setFlag(uint64_t FLAG);
	/**
	 * \brief Clears a flag on this Connection object.
	 * @param FLAG
	 */
	void clearFlag(uint64_t FLAG);
	/**
	 * \brief Checks a flag on this Connection object.
	 * @param FLAG
	 * @return
	 */
	bool checkFlag(uint64_t FLAG);
	/**
	 * \brief Receive waiting data and move it to an internally managed buffer.
	 *
	 * This function is intended to accept incoming data and move it to a buffer while
	 * waiting on a retrieval call from an outside function. Essentially, this is intended
	 * to allow data to be received at top speed while processing functions are tied up in
	 * another thread. The internal buffer will grow to a maximum size Connection::MAX_BUFFER_SIZE
	 * which is currently defined as 64 megabytes. Attachments aside, no email message
	 * sent through this system should ever reach 64 megabytes in size. In theory. With the
	 * increasing RAM sizes available on computers and servers alike, 64 MB is a reasonable
	 * maximum size for any buffered content and should allow for better processing speed.
	 * In practice, this function may rarely be called, and the though the clearing of the
	 * buffer may be completely transparent to the calling function.
	 * @return true if successful, false if the buffer has reached maximum capacity.
	 */
	bool bufferWaitingData();
	/**
	 * \brief Checks to see if data is waiting on the socket.
	 *
	 * This function checks to see if there is data waiting to be recv()'d on the socket
	 * and returns the number of bytes pending if there is.
	 * @param timeout_sec
	 * @param timeout_msec
	 * @return
	 */
	int32_t dataWaiting(int32_t timeout_sec = 0, int32_t timeout_msec = 0);
	/**
	 *
	 * @param A
	 * @param addrStr
	 * @return
	 */
	static char *getIPv4(struct sockaddr_in6 *A, char *addrStr = NULL);
	/**
	 *
	 * @param FLAG
	 */
	static void setConfigFlag(uint64_t FLAG);
	/**
	 *
	 * @param FLAG
	 */
	static void clearConfigFlag(uint64_t FLAG);
	/**
	 *
	 * @param FLAG
	 * @return
	 */
	static bool checkConfigFlag(uint64_t FLAG);
	static const uint64_t CFG_FLAG_ALLOW_INSECURE_CONNECTIONS = 0x0000000000000001; //!< Allow connections without SSL support; this should only be allowed for testing and development.
	const uint64_t MAX_BUFFER_SIZE = 0x4000000; //!< Maximum buffer size is 64 MB. God forbid it ever get that large before processing.
};

} /* namespace AMS */
} /* namespace BLS */

#endif /* SRC_CONNECTION_H_ */
