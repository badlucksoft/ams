/*
 * Connection.cpp
 *
 *  Created on: Sep 10, 2016
 *      Author: rrodgers
 */

#include "Connection.h"
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
namespace BLS {
namespace AMS {
uint64_t Connection::cfgFlags = 0L;
Connection::Connection() {
	// TODO Auto-generated constructor stub
	this->init();
}

void Connection::init(void) {
	this->flags = 0L;
	memset(&this->cli,0,sizeof(this->cli));
	this->socket = 0;
	this->cliLen = 0;
	memset(&this->address,0,47);
	this->hostname = NULL;
	this->buffer = NULL;
	this->bufferedDataLen = 0L;
	this->totalBytesReceived = 0L;
	this->totalBytesSent = 0L;
}

Connection::Connection(struct sockaddr_in6 CLI, socklen_t CLILEN,int32_t SOCKET) {
	this->init();
	this->cliLen = CLILEN;
	memcpy(&this->cli,&CLI,this->cliLen);
	this->socket = SOCKET;
	if( IN6_IS_ADDR_V4MAPPED(&this->cli.sin6_addr) == 1 ) this->setFlag(Connection::FLAG_IPV4_MAPPED);
	if( this->checkFlag(Connection::FLAG_IPV4_MAPPED) && Connection::getIPv4(&this->cli,this->address) != NULL)
	{
		printf("Connection class; IPv4 address %s\n",this->address);
	}
	else if( this->checkFlag(Connection::FLAG_IPV4_MAPPED) == false)
	{
		inet_ntop(AF_INET6,&this->cli.sin6_addr,this->address,46);
		printf("Connection class; IPv6 address %s\n",this->address);
	}
	struct hostent *hostp = gethostbyaddr((const char *)&cli.sin6_addr.s6_addr,sizeof(cli.sin6_addr.s6_addr),AF_INET);
	if( hostp != NULL)
	{
		uint16_t len = strlen(hostp->h_name);
		this->hostname = (char*)calloc(len+1,1);
		strncpy(this->hostname,hostp->h_name,len);
		printf("Hostname: %s\n",this->hostname);
	}
	int32_t waiting = this->dataWaiting();
	if(waiting > 0)
	{
		this->bufferWaitingData();
		/*
		char *data = (char*)calloc(waiting+1,1);
		recv(this->socket,data,waiting,0);
		if( strlen(data) > 0) printf("received %d bytes from remote:\n%s\n",waiting,data);
		memset(data,0,waiting+1);
		free(data);
		*/
	}
	char *message = (char*) calloc(200,1);
	sprintf(message,"Hello %s, thank you for connecting\r\n",this->address);
	send(this->socket,message,strlen(message),0);
	free(message);
}

Connection::~Connection() {
	// TODO Auto-generated destructor stub
	close(this->socket);
	if( this->hostname != NULL)
	{
		memset(this->hostname,0,strlen(this->hostname));
		free(this->hostname);
		this->hostname = NULL;
	}
	if( this->buffer != NULL)
	{
		memset(this->buffer, 0, this->bufferedDataLen);
		free(this->buffer);
		this->buffer = NULL;
		this->bufferedDataLen = 0L;
	}
	memset(&this->cli,0,sizeof(this->cli));
}
bool Connection::bufferWaitingData()
{
	bool success = false;
	uint64_t waitingData = this->dataWaiting();
	if( waitingData > 0 && (this->bufferedDataLen + waitingData) < Connection::MAX_BUFFER_SIZE )
	{
		uint64_t receivedBytes = 0L;
		if(this->buffer == NULL && this->bufferedDataLen == 0)
		{
			this->buffer = (unsigned char*)calloc(waitingData,1);
			this->bufferedDataLen = recv(this->socket,this->buffer,waitingData,0);
		}
		else if( this->buffer != NULL && this->bufferedDataLen > 0)
		{
			this->buffer = (unsigned char*)realloc(this->buffer,this->bufferedDataLen+waitingData);
			memset(this->buffer+this->bufferedDataLen,0,waitingData);
			receivedBytes = recv(this->socket,this->buffer+this->bufferedDataLen,waitingData,0);
			this->bufferedDataLen += receivedBytes;
		}
		success = true;
		printf("buffered %ld bytes (%ld total buffered)\n",waitingData,this->bufferedDataLen);
		if( receivedBytes > 0) this->totalBytesReceived += receivedBytes;
	}
	return success;
}
int32_t Connection::dataWaiting(int32_t timeout_sec, int32_t timeout_msec)
{
	int32_t result = 0;
	fd_set read, except;
	FD_ZERO(&read);
	FD_ZERO(&except);
	FD_SET(this->socket,&read);
	FD_SET(this->socket,&except);
	select(this->socket+1,&read,NULL,&except,NULL);
	if(FD_ISSET(this->socket,&read))
	{
		ioctl(this->socket,FIONREAD,&result);
	}
	return result;
}
void Connection::setFlag(uint64_t FLAG) {
	this->flags |= FLAG;
}

void Connection::clearFlag(uint64_t FLAG) {
	this->flags ^= FLAG;
}

bool Connection::checkFlag(uint64_t FLAG) {
	return (this->flags & FLAG) == FLAG;
}
char *Connection::getIPv4(struct sockaddr_in6 *A, char *addrStr)
{
	char *ret = NULL;
	const uint8_t *bytes = ((const struct sockaddr_in6 *)A)->sin6_addr.s6_addr;
	bytes += 12;
	struct in_addr addr = {*(const in_addr_t *)bytes};
	const char *other = NULL;
	if( addrStr == NULL)
	{
	ret = (char*) calloc(17,0);
	other = inet_ntop(AF_INET,&addr,ret,16);
	}
	else
	{
		other = inet_ntop(AF_INET,&addr,addrStr,16);
		ret = addrStr;
	}
//	printf("getIPv4: %s\t%s\n",other,ret);
	return ret;
}

void Connection::setConfigFlag(uint64_t FLAG) {
	Connection::cfgFlags |= FLAG;
}

void Connection::clearConfigFlag(uint64_t FLAG) {
	if( (Connection::cfgFlags & FLAG) == FLAG) Connection::cfgFlags ^= FLAG;
}

bool Connection::checkConfigFlag(uint64_t FLAG) {
	return (Connection::cfgFlags & FLAG) == FLAG;
}

} /* namespace AMS */
} /* namespace BLS */
