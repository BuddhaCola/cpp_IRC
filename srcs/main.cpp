#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <poll.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <iostream>


int startup( int port )
{
    // 1. Create sockets
    int listening = socket(AF_INET,SOCK_STREAM,0);//The second parameter here represents TCP
    if( listening < 0 )
    {
        std::cerr << "Sockek fail.." << std::endl;
        exit(2);
    }

    std::cout << "Hello World!" << std::endl;

    // 2. When TIME_WAIT is resolved, the server cannot be restarted; the server can be restarted immediately.
    int opt = 1;
    if (fcntl(listening, F_SETFL, O_NONBLOCK))
        throw "Non-blocking socket isn't made";
    setsockopt(listening,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

    struct sockaddr_in local;
    local.sin_family = AF_INET;
    local.sin_addr.s_addr = htonl(INADDR_ANY);// Address of any type
    local.sin_port = htons(port);// The port number here can also be specified directly as 8080.
    // 3. Binding port number

    if( bind(listening,(struct sockaddr *)&local,sizeof(local)) < 0 )
    {
        std::cerr << "Bind fail.." << std::endl;
        exit(3);
    }

    // 4. Get a listening socket
    if( listen(listening,5) < 0 )
    {
        std::cerr << "listen fail... " << std::endl;
        exit(4);
    }
    return listening;
}

int main(int argc,char* argv[] )
{
    if( argc != 2 )
    {
        std::cout << argv[0] << " and <port> number!" << std::endl;
        return 1;
    }


    // 1. Get a listening socket
    int listen_sock = startup(atoi(argv[1]));//Port numbers are passed in as strings and need to be converted to integers


    // 2. Initialization of Structures - List of Structures to Monitor
    struct pollfd fd_list[1024];
    int fd_list_size = sizeof(fd_list)/sizeof(fd_list[0]);
    int i = 0;

    for(; i < fd_list_size ; i++  )
    {
        fd_list[i].fd = -1;// File descriptor
        fd_list[i].events = 0;// Set of events to monitor
        fd_list[i].revents = 0;// Ready Event Set of Concerned Descriptors
    }


    // 3. Add read-only events for file descriptors of interest

    i = 0;
    for( ; i < fd_list_size; i++ )
    {
        if( fd_list[i].fd == -1 )
        {
            fd_list[i].fd = listen_sock;
            fd_list[i].events = POLLIN;// Concern about Read-Only Events
            break;
        }

    }
    while(1)
    {

        //4. Start calling poll and wait for the file descriptor set of interest to be ready
        switch( poll(fd_list,fd_list_size,3000) )
        {
            case 0:// The state of the denominator has exceeded before it has changed. timeout Time
                //printf("timeout...\n");
                continue;
            case -1:// failed
                std::cout << "poll fail ..." << std::endl;
                continue;
            default: // Succeed
            {
                //   If it is a listener file descriptor, call accept to accept a new connection
                //   If it is a normal file descriptor, read is called to read the data
                int i = 0;
                for( ;i < fd_list_size; i++ )
                {
                    if( fd_list[i].fd == -1 )
                    {
                        continue;
                    }
                    if( fd_list[i].fd == listen_sock && ( fd_list[i].revents & POLLIN ))
                    {
                        // 1. Provide a connection acceptance service if the listening socket is ready to read

                        struct sockaddr_in client_adr;
                        socklen_t len = sizeof(client_adr);
                        int new_sock = accept(listen_sock,(struct sockaddr *)&client_adr,&len);
                        if(new_sock < 0)
                        {
                            std::cout << "accept fail ..." << std::endl;
                            continue;
                        }
                        //After obtaining the new file descriptor, add the file descriptor to the array for the next time you care about the file descriptor
                        int i = 0;
                        for( ; i < fd_list_size; i++ )
                        {
                            if( fd_list[i].fd == -1 )//Place the first value in the array at - 1
                                break;
                        }
                        if( i < fd_list_size )
                        {
                            fd_list[i].fd = new_sock;
                            fd_list[i].events = POLLIN;
                        }
                        else
                        {
                            close(new_sock);
                        }
                        std::cout << "Client successfully connected " << inet_ntoa(client_adr.sin_addr) << ":"
                                << ntohs(client_adr.sin_port) << std::endl;
                        continue;
                    }

                    //2. At this point, we are concerned with ordinary file descriptors.
                    //   Provide services to read data at this time
                    if( fd_list[i].revents & POLLIN  )
                    {
                        char buf[1024];
                        ssize_t s = read(fd_list[i].fd,buf,sizeof(buf)-1);
                        if( s < 0 )
                        {
                            std::cout << "read fail ..." << std::endl;
                            continue;
                        }
                        else if( s == 0 )
                        {
                            std::cout << "client#" << i << " quit" << std::endl;
                            close(fd_list[i].fd);
                            fd_list[i].fd = -1;
                        }
                        else
                        {
                            buf[s] = 0;
                            std::cout << "client fd_list[" << i << "]: " << buf << std::endl;
                        }
                    }
                }
            }
                break;
        }
    }
    return 0;
}
