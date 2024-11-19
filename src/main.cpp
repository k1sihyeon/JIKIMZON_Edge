#include "v4l2Handler.hpp"
#include "tcpHandler.hpp"

int main(void)
{
    TcpHandler::GetInstance()->InitSocket();
    V4L2Handler::GetInstance()->StartCapture();
}