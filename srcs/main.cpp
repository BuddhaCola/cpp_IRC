#include "../Server.hpp"
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <poll.h>
#include <string>

using namespace std;

int start_up( int port)
{
// Create a socket
	int listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening < 0)
	{
		cerr << "Can't create a socket! Quitting" << endl;
		return -1;
	}
	int opt = 1;
	setsockopt(listening,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

// Bind the ip address and port to a socket
	struct sockaddr_in sock_ip;
	sock_ip.sin_family = AF_INET;
	sock_ip.sin_addr.s_addr = htonl(INADDR_ANY);
	sock_ip.sin_port = htons(port);

	if (bind(listening, (struct sockaddr *) &sock_ip, sizeof(sock_ip)) < 0)
	{
		cerr << "Can't bind a socket!" << endl;
		return -2;
	}


// Tell Winsock the socket is for listening
	if (listen(listening, 5) < 0)
	{
		cerr << "Can't listen" << endl;
		return -3;
	}
	return listening;
}



int main (int argc, char* argv[])
{
	if (argc != 2)
	{
		cout << argv[0] << ", Please enter port" << endl;
		return 1;
	}

	// Get a listening socket
	int listen_sock = start_up(atoi(argv[1]));
	// List of Structures to Monitor
	struct pollfd fd_list[1024];
	int clientSize = sizeof(fd_list) / sizeof(fd_list[0]);
	for (int i = 0; i < clientSize; i++)
	{
		fd_list[i].fd = -1; // File descriptor
		fd_list[i].events = 0; // Set of events to monitor
		fd_list[i].revents = 0; // Ready Event Set of Concerned Descriptors
	}

	//Add read-only events for file descriptors of interest
	for (int i = 0; i < clientSize; i++)
	{
		if (fd_list[i].fd == -1)
		{
			fd_list[i].fd = listen_sock;
			fd_list[i].events = POLLIN;    // Concern about Read-Only Events
			break;
		}
	}
	while (1)
	{
		//Start calling poll
		switch (poll(fd_list, clientSize, 3000))
		{
			case 0: // timeout Time
				cout << "it's alive..." << endl;
				continue;
			case -1: //failed
				cout << "poll fail..." << endl;
				continue;
			default: //Succed
			{
				// If it's a listener file descriptor, call accept to accept
				// a new connection
				for (int i = 0; i < clientSize; i++)
				{
					if (fd_list[i].fd == -1)
					{
						continue;
					}
					if (fd_list[i].fd == listen_sock && (fd_list[i].revents
						&POLLIN))
					{
						// Provide a connection acceptance sevice if the
						// listening socket is ready to read
						struct sockaddr_in client;
						socklen_t len = sizeof(client);
						int new_sock = accept(listen_sock, (struct sockaddr
								*) &client, &len);
						if (new_sock < 0)
						{
							cerr << "accept fail" << endl;
							continue;
						}
						//After obtaining the new file descriptor, add the file descriptor to the array for the next time you care about the file descriptor
						for (int i = 0; i < clientSize; i++)
						{
							if (fd_list[i].fd == -1) // Place the first value
								// in the array at -1
								break;
						}
						if (i < clientSize)
						{
							fd_list[i].fd = new_sock;
							fd_list[i].events = POLLIN;
						} else
						{
							close(new_sock);
						}
						cout << "get a new link! " << inet_ntoa(client.sin_addr)
							 << "; " << ntohs(client.sin_port) << endl;
						continue;
					}
					//At this point, we are concerned with ordinary file
					// descriptors.
					if (fd_list[i].revents & POLLIN)
					{
						char buf[1024];
						ssize_t s = read(fd_list[i].fd, buf, sizeof(buf) - 1);
						if (s < 0)
						{
							cerr << "read fail..." << endl;
							continue;
						} else if (s == 0)
						{
							cout << "client quit..." << endl;
							close(fd_list[i].fd);
							fd_list[i].fd = -1;
						} else
						{
							buf[s] = 0;
							cout << "client: " << buf << endl;
						}
					}
				}
			}
				break;
		}
	}
	return 0;
}