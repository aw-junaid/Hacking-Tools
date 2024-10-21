#include "stdafx.h"
#include "PORTSCAN.h"

#include <string.h>
#include <winsock2.h>
#include <ws2def.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <vector>
#include <iostream>
#include <fstream>

#pragma comment(lib, "ws2_32.lib")
#include <string>

void scanFile(std::string filename)
{
	std::ofstream myfile;
	myfile.open("truc.txt");

	std::ifstream of;
	std::vector<std::string> vector_string;
	of.open(filename);

	while (!of.eof())
	{
		// read an entire line into memory
		char buf[200];
		of.getline(buf, 200);

		// parse the line into blank-delimited tokens
		int n = 0; // a for-loop index

		// array to store memory addresses of the tokens in buf
		const char* token[200] = {}; // initialize to 0

		// parse the line
//		token[0] = strtok(buf, " "); // first token
		if (token[0]) // zero if line is blank
		{
			for (n = 1; n < 200; n++)
			{
//				token[n] = strtok(0, " "); // subsequent tokens
				if (!token[n]) break; // no more tokens
			}
		}

		// process (print) the tokens
		for (int i = 0; i < n; i++) // n = #of tokens
		{
			if (strcmp(token[i], "LISTENING") == 0 || strcmp(token[i], "ETABLISHED") == 0)
			{
				i++;
				if (std::find(vector_string.begin(), vector_string.end(), token[i]) == vector_string.end())
				{
					vector_string.push_back(token[i]);
					myfile << vector_string[vector_string.size() - 1] << std::endl;
				}
			}
		}
	}
	myfile.close();

	std::ofstream myfile2;
	myfile2.open("scanports.txt");

	SOCKET my_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	sockaddr_in addr;

	ZeroMemory(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(14780);
	
	int i = bind(my_sock, (sockaddr*)&addr, sizeof(addr));
	myfile2 << i << " YOLO" << std::endl;

	myfile2.close();

	closesocket(my_sock);
}

void PortScan()
{
	std::ofstream myfile;
	myfile.open("ports.txt");
	std::vector<int> ports = parseRangePort(0, 65535); // List of ports
	std::vector<int> open_ports; // List of open ports

	WSADATA my_first_sock;
	SOCKET sockett;
	SOCKADDR_IN sa;

	memset(&sa, 0, sizeof sa);

	struct addrinfo addr;
	memset(&addr, 0, sizeof(addrinfo));
	addr.ai_family = AF_INET ;
	addr.ai_socktype = SOCK_STREAM ;
	addr.ai_protocol = 0;
	addr.ai_flags = AI_PASSIVE ;

	struct addrinfo *rp, *result;
	getaddrinfo("localhost", "8080", &addr, &result);

	WSAStartup(MAKEWORD(2, 2), &my_first_sock);

	SOCKET client = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);

	sa.sin_family = AF_INET ;
	sa.sin_port = htons(8080);
//	sa.sin_addr = htonl(0x7f000001); //127.0.0.1
	// We need to connect to a closed port, socket state must be CONNECTING
	int error = connect(client, (SOCKADDR*)&sa, sizeof(sa));

	myfile << error << "plopi" << std::endl;
	struct hostent* host;
	host = gethostbyname("127.0.0.1");

	sa.sin_family = AF_INET ;
	// Define socket host
	sa.sin_addr.s_addr = INADDR_ANY;// ((struct in_addr *)host->h_addr_list)->s_addr;
	sa.sin_port = htons(8080);
	//sa.sin_addr.s_addr = *((unsigned long*)host->h_addr);

	//need a while loop to check open ports every x time

	for (int port : ports)
	{
		sockett = socket(AF_INET, SOCK_STREAM, 0);
		if (sockett < 0)
		{
			exit(EXIT_FAILURE); //ERROR
			myfile << "socket error on port " << port << "\n";
		}

		char* opval = NULL;
		setsockopt(sockett, SOL_SOCKET, SO_REUSEADDR, opval, sizeof(opval));

		// Connect socket
		int err = connect(sockett, (SOCKADDR*)&sa, sizeof(sa));
		myfile << err << std::endl;
		if (err == SOCKET_ERROR)
		{
			fflush(stdout);
			myfile << "error to connect socket on port " << port << "\n";
		}
		else
		{
			myfile << "Connected to " << port << "\n";

			// Add open port to the list
			open_ports.push_back(port);
		}
		myfile << "Closing socket \n";
		WSACleanup();
		// Close the socket
		closesocket(sockett);
	}

	myfile << open_ports.size() << std::endl;

	for (int por : open_ports)
	{
		myfile << (char*)por << std::endl;
		sa.sin_port = por;
		connect(sockett, (struct sockaddr *)&sa, sizeof sa);
		int rv = send(sockett, "lolilol", 7, 0); // This is a test, send lolilol to each port in the list
		std::cout << rv << " \n";
		closesocket(sockett);
	}
	myfile.close();
	/*sf::TcpSocket socket;
	for (int por : open_ports)
	{
	socket.connect("localhost", por);
	socket.send();
	}*/
}


bool isPortOpen(const std::string& add, int port)
{
	/*SOCKET sock;

	bool is_open = (sock.connect(sf::IpAddress(add), port) == SOCKET_ERROR);
	sock.disconnect();
	return is_open;	*/
	return false;
}


static std::vector<int> parseRangePort(int min, int max)
{
	// Fill the list with each number port
	std::vector<int> ports;
	for (; min <= max; ++min)
		ports.push_back(min);
	return ports;
}

void sendFile(std::vector<int> open_ports, std::string filename)
{
	SOCKET sock; /* socket */
	int desc; /* file descriptor for socket */
	int fd; /* file descriptor for file to send */
	struct stat stat_buf; /* argument to fstat */
	struct sockaddr_in addr; /* socket parameters for bind */
	struct sockaddr_in addr1; /* socket parameters for accept */

	/* create socket */
	sock = socket(AF_INET, SOCK_STREAM, 0);
	/*if (sock == -1) {
		myfile << "unable to create socket: %s\n", strerror(errno));
		exit(1);
	}*/

	/* fill in socket structure */
	for (int port : open_ports)
	{
		memset(&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET ;
		addr.sin_addr.s_addr = INADDR_ANY;
		addr.sin_port = htons(port);

		/* bind socket to the port */
		int rc = bind(sock, (struct sockaddr *)&addr, sizeof(addr));
		/*		if (rc == -1) {
			fprintf(stderr, "unable to bind to socket: %s\n", strerror(errno));
			exit(1);
		}*/

		/* listen for clients on the socket */
		rc = listen(sock, 1);
		/*		if (rc == -1) {
			fprintf(stderr, "listen failed: %s\n", strerror(errno));
			exit(1);
		}*/

		/* open the file to be sent */

		/*		FILE *f = fopen(filename.c_str(), "rb");
		fseek(f, 0, SEEK_END);
		long fsize = ftell(f);
		fseek(f, 0, SEEK_SET);
		                                       HERE
		char *buff = new char[fsize + 1];
		fread(buff, fsize, 1, f);
		fclose(f);

		buff[fsize] = 0;*/

		/*fd = open(filename, O_RDONLY);
		if (fd == -1) {
		fprintf(stderr, "unable to open '%s': %s\n", filename, strerror(errno));
		exit(1);
		}*/

		/* get the size of the file to be sent */
		//fstat(fd, &stat_buf);

		/* copy file using sendfile */
		//int offset = 0;

		// send the file via socket


		//rc = send(sock, buff, fsize + 1, 0);     HERE

		//rc = sendfile(desc, fd, &offset, stat_buf.st_size);

		if (rc == SOCKET_ERROR)
		{
			//fprintf(stderr, "error from send: %s\n", strerror(errno));
			exit(1);
		}
		/*if (rc != stat_buf.st_size)
		{
		fprintf(stderr, "incomplete transfer from sendfile: %d of %d bytes\n",
		rc,
		(int)stat_buf.st_size);
		exit(1);
		}*/

		/* close descriptor for file that was sent */
		//close(fd);

		/* close socket descriptor */
		//close(desc);

		/* close socket */
		closesocket(sock);
	}
}
