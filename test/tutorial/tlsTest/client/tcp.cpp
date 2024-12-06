#include "tcp.hpp"

TCP::TCP()
{   
    mHostname = "192.168.10.99";
    mPort = 4433;
}

TCP::~TCP()
{   
    if (mClientFd)
    {
        close(mClientFd);
    }
}

int TCP::CreateClientSocket() {
    mClientFd = socket(AF_INET, SOCK_STREAM, 0);
    if (mClientFd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    mServerAddr.sin_family = AF_INET;
    mServerAddr.sin_port = htons(mPort);

    if (inet_pton(AF_INET, mHostname, &mServerAddr.sin_addr) <= 0) {
        perror("Invalid address or address not supported");
        close(mClientFd);
        exit(EXIT_FAILURE);
    }

    if (connect(mClientFd, (struct sockaddr*)&mServerAddr, sizeof(mServerAddr)) < 0) {
        perror("Connection failed");
        close(mClientFd);
        exit(EXIT_FAILURE);
    }

    return mClientFd;
}