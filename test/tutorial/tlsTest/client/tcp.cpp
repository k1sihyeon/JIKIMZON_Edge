#include "tcp.hpp"
#include <iostream>

TCP::TCP()
	: mHostname("192.168.10.99")
	, mPort (12345)
{   
	std::cout << mHostname << std::endl;
	std::cout << mPort << std::endl;
}

TCP::~TCP()
{   
    if (mSocketFd)
    {
        close(mSocketFd);
    }
}

int TCP::CreateClientSocket() {
    mSocketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (mSocketFd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
	std::cout << "socket() success" << std::endl;

	sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(mPort);

    if (inet_pton(AF_INET, mHostname.c_str(), &serverAddr.sin_addr) <= 0) {
        perror("Invalid address or address not supported");
        close(mSocketFd);
        exit(EXIT_FAILURE);
    }
	else {
		std::cout << "setup" << std::endl;
	}

    if (connect(mSocketFd, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Connection failed");
        close(mSocketFd);
        exit(EXIT_FAILURE);
    }
	std::cout << "connect() success" << std::endl;


    return mSocketFd;
}