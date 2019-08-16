#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <netdb.h>
#include <iostream>
#include <cstring>

void error(std::string msg, unsigned int exit_code) {
	std::cerr << msg << std::endl;
	exit(exit_code);
}

void error(std::string msg) {
	error(msg, 1);
}

int main() {
	int target_port = 6718;
	const std::string target_host = "localhost";

	int client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(client_socket < 0)
		error("Unable to open socket");

	hostent* host = gethostbyname(target_host.c_str());
	if(host == NULL)
		error("can't find host");

	sockaddr_in sockinfo;
	bzero((char*)&sockinfo, sizeof(sockinfo));
	sockinfo.sin_family = AF_INET;
	
	bcopy((char*)host->h_addr, (char*)&sockinfo.sin_addr.s_addr, host->h_length);
	sockinfo.sin_port = htons(target_port);

	int connect_attempt = connect(client_socket, (sockaddr*)&sockinfo, sizeof(sockinfo));
	if(connect_attempt < 0)
		error("Unable to connect");

	
	std::string buffer = "Hello, from the git-server-proxy";

	int write_bytes = write(client_socket, buffer.c_str(), buffer.size());
	if(write_bytes < 0)
		error("Send failed");

	close(connect_attempt);

	return 0;
}
