#include "tcpHandler.hpp"


TcpHandler* TcpHandler::instance = nullptr;


TcpHandler::TcpHandler() 
{
}

TcpHandler::~TcpHandler() {}

void TcpHandler::InitSocket() {
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
	{
		perror("socket");
		exit(EXIT_FAILURE);
	}

    // Bind socket
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(PORT);
	if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		perror("bind");
		exit(EXIT_FAILURE);
	}

	// Listen for incoming connections
	listen(sockfd, 5);
	printf("Waiting for a client to connect...\n");

	// Accept a connection from client
	clientSock = accept(sockfd, (struct sockaddr *)&client_addr, &client_len);
	if (clientSock < 0)
	{
		perror("accept");
		exit(EXIT_FAILURE);
	}
	printf("Client connected!\n");
}

void TcpHandler::SendFrame(cv::Mat frame)
{
    if (clientSock < 0)
    {
        std::cerr << "not valid sock fd" << std::endl;
        exit(EXIT_FAILURE);
    }

    int imgSize = frame.total() * frame.elemSize();

    if (send(clientSock, frame.data, imgSize, 0) < 0)
    {
        perror("send");
    }
}