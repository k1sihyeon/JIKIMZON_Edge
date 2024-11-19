#include "tcpHandler.hpp"

void TcpHandler::InitSocket()
{
  mSockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (mSockfd < 0)
	{
		std::cerr << "socket: msockfd" << std::endl;
		exit(EXIT_FAILURE);
	}

  // Bind socket
	mServerAddr.sin_family = AF_INET;
	mServerAddr.sin_addr.s_addr = INADDR_ANY;
	mServerAddr.sin_port = htons(PORT);
	if (bind(mSockfd, (struct sockaddr *)&mServerAddr, sizeof(mServerAddr)) < 0)
	{
		perror("bind");
		exit(EXIT_FAILURE);
	}

	// Listen for incoming connections
	listen(mSockfd, 5);
	printf("Waiting for a client to connect...\n");

	// Accept a connection from client
	mClientSock = accept(mSockfd, (struct sockaddr *)&mClientAddr, &mClientSockLen);
	if (mClientSock < 0)
	{
		perror("accept");
		exit(EXIT_FAILURE);
	}
	printf("Client connected!\n");
}

void TcpHandler::SendFrame(const std::vector<uchar>& frame)
{
    if (mClientSock < 0)
    {
        std::cerr << "not valid client sock" << std::endl;
        exit(EXIT_FAILURE);
    }

    if (send(mClientSock, frame.data(), frame.size(), 0) < 0)
    {
        perror("send");
    }
}