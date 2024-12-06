#include "tls.hpp"
#include "tcp.hpp"

int main() {
    TLS* tls = new TLS();
    TCP* tcp = new TCP();
    
    int clientFd = tcp->CreateClientSocket();
    std::cout << "CreateClientSocket()" << std::endl;




    tls->Init();
    std::cout << "INIT()" << std::endl;




    tls->PerformTLSHandshake(clientFd);
	std::cout << "PerformTLSHandshake()" << std::endl;



    tls->CommunicateWithServer();
	std::cout << "CommunicateWithServer()" << std::endl;
	


    delete tls;
    delete tcp;

	
    return 0;
}