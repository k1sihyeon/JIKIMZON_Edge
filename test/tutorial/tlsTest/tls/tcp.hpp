#ifndef JIKIMZON_TCP_H
#define JIKIMZON_TCP_H


class TCP {
public:
    TCP();
    ~TCP();
    
private:
    int mPort;
    int mSocket;
    void createServerSocket();
};

#endif