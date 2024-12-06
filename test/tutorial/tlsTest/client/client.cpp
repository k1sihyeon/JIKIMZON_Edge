#include "tls.hpp"
#include "tcp.hpp"

int main() {
    TLS* tls = new TLS();
    TCP* tcp = new TCP();
    
    int clientFd = tcp->CreateClientSocket();
    tls->Init();

    tls->PerformTLSHandshake(clientFd);
    tls->CommunicateWithServer();

    delete tls;
    delete tcp;
    return 0;
}