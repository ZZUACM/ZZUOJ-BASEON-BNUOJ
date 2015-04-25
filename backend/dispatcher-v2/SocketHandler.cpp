/* 
 * File:   SocketHandler.cpp
 * Author: 51isoft
 * 
 * Created on 2014年1月13日, 下午10:06
 */

#include "SocketHandler.h"

const int SocketHandler::CHECK_ALIVE_INTERVAL = 10;

SocketHandler::SocketHandler(int _sockfd):sockfd(_sockfd)
{
	last_check = time(NULL);
}

SocketHandler::~SocketHandler()
{
	close(sockfd);
}

/**
 * Check whether the socket is still alive
 * @return Is alive or not
 */
bool SocketHandler::checkAlive()
{
	if (time(NULL) - last_check < CHECK_ALIVE_INTERVAL)
		return true;
	char data;
	ssize_t bytes = recv(sockfd, &data, 1, MSG_PEEK | MSG_DONTWAIT);	//read one byte
	if (bytes == 0)
		return false;
	last_check = time(NULL);
	return true;
}

/**
 * Get connection message to identify the socket
 * @return The connection message, within HANDSHAKE_TIMEOUT
 */
string SocketHandler::getConnectionMessage()
{

	struct timeval case_startv, case_nowv;
	struct timezone case_startz, case_nowz;
	gettimeofday(&case_startv, &case_startz);

	int time_passed;
	ssize_t length;
	char buffer[255];
	while (1) {
		usleep(10000);
		gettimeofday(&case_nowv, &case_nowz);
		time_passed =
		    (case_nowv.tv_sec - case_startv.tv_sec) * 1000 +
		    (case_nowv.tv_usec - case_startv.tv_usec) / 1000;
		if ((length = recv(sockfd, buffer, 255, MSG_DONTWAIT)) > 0
		    || time_passed > HANDSHAKE_TIMEOUT)
			break;
	}
	buffer[length] = 0;
	return buffer;
}

/**
 * Send a message without any prepend length info
 * @param message       Message to be sent, use const char *
 * @param length        Length of the message
 */
void SocketHandler::sendMessage(const char *message, size_t length)
{
	size_t sent = 0;
	for (sent = 0; sent < length;) {
		ssize_t current_sent = write(sockfd, message, length - sent);
		if (current_sent == -1) {
			throw(Exception("Connection lost"));
		} else {
			sent += current_sent;
		}
	}
}

/**
 * Send a message without any prepend length info
 * @param message       Message to send
 */
void SocketHandler::sendMessage(string message)
{
	sendMessage(message.c_str(), message.length());
}

/**
 * Send file without prepend length info
 * @param filename      File to send
 */
void SocketHandler::sendFileWithoutLength(string filename)
{
	int source = open(filename.c_str(), O_RDONLY);
	size_t temp_length = 0;
	char buffer[1024];
	while ((temp_length = read(source, buffer, sizeof(buffer))) > 0) {
		try {
			sendMessage(buffer, temp_length);
		}
		catch(Exception & e) {
			close(source);
			throw e;
		}
	}
	close(source);
}

/**
 * Send file, prepend length info
 * @param filename      File to send
 */
void SocketHandler::sendFile(string filename)
{
	int source = open(filename.c_str(), O_RDONLY);
	struct stat file_stat;
	char buffer[1024];

	fstat(source, &file_stat);
	size_t length = htonl(file_stat.st_size);

	// use first 4 bytes to send file size
	memcpy(buffer, &length, sizeof(length));
	try {
		sendMessage(buffer, sizeof(length));
	}
	catch(Exception & e) {
		close(source);
		throw e;
	}

	// send the message body
	while ((length = read(source, buffer, sizeof(buffer))) > 0) {
		try {
			sendMessage(buffer, length);
		}
		catch(Exception & e) {
			close(source);
			throw e;
		}
	}
	close(source);
}

/**
 * Receive message from the socket, store them to buffer
 * @param buffer        Message buffer
 * @param length        The length of the buffer
 * @return Number of bytes received
 */
size_t SocketHandler::receiveMessage(char *buffer, size_t length)
{
	ssize_t got;
	got = recv(sockfd, buffer, length, 0);
	if (got == 0) {
		throw(Exception("Connection lost"));
	}
	return got;
}

/**
 * Receive a file from the socket
 * @param filename      Filename to store
 */
void SocketHandler::receiveFile(string filename)
{
	FILE *fp;
	size_t length, got;
	char buffer[1024];

	// get first 4 bytes, the length info
	receiveMessage(buffer, sizeof(length));
	memcpy(&length, buffer, sizeof(length));
	length = ntohl(length);

	// recieve body, write them to file
	fp = fopen(filename.c_str(), "wb");
	got = 0;
	while (got < length) {
		size_t current_got;
		try {
			current_got =
			    receiveMessage(buffer,
					   min(length - got, sizeof(buffer)));
		}
		catch(Exception & e) {
			fclose(fp);
			throw e;
		}
		got += current_got;
		fwrite(buffer, sizeof(char), current_got, fp);
	}
	fclose(fp);
}

void SocketHandler::receiveFileWithoutLength(string filename)
{
	bool got_things = false;
	ssize_t got;
	FILE *fp;
	char buffer[1024];

	fp = fopen(filename.c_str(), "wb");
	while (!got_things) {
		buffer[0] = 0;	// clear first char, for sanity check
		// use non-blocking method since we don't know the file size
		while ((got = recv(sockfd, buffer, 1024, MSG_DONTWAIT)) > 0) {
			got_things = true;
			fwrite(buffer, sizeof(char), got, fp);
		}
		if (got == 0) {
			fclose(fp);
			throw(Exception("Connection lost"));
		}
		usleep(67354);	// sleep for a random time
		// sanity check, ensure it's the message we need
		if (buffer[0] != '<')
			got_things = false;
	}
	fclose(fp);
}
