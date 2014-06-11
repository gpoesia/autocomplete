#include <sys/types.h>
#include <sys/socket.h>
#include <sys/io.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>
#include <cstring>

#include "connectionpeer.h"

ConnectionPeer::ConnectionPeer() :
    socketFileDescriptor_(-1),
    buffer_()
{
}

bool ConnectionPeer::connect(std::string serverHostName, int port)
{
    addrinfo *serverAddrInfo;
    addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = AF_INET;

    std::string portAsString = std::to_string(port);

    if (getaddrinfo(serverHostName.data(), portAsString.data(),
                &hints, &serverAddrInfo) != 0)
        return false;

    sockaddr_in serverAddress;

    socketFileDescriptor_ = socket(AF_INET, SOCK_STREAM, 0);

    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr =
            reinterpret_cast<sockaddr_in*>(serverAddrInfo->ai_addr)->sin_addr.s_addr;
    serverAddress.sin_port = htons(port);

    freeaddrinfo(serverAddrInfo);

    bool success = ::connect(socketFileDescriptor_,
            reinterpret_cast<sockaddr*>(&serverAddress),
            sizeof serverAddress) == 0;

    return success;
}

bool ConnectionPeer::listen(int port)
{
    socketFileDescriptor_ = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serverAddress;

    memset(&serverAddress, 0, sizeof serverAddress);
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);

    int yes = 1;
    setsockopt(socketFileDescriptor_, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

    if (bind(socketFileDescriptor_, reinterpret_cast<sockaddr*>(&serverAddress), sizeof serverAddress) != 0)
        return false;

    return ::listen(socketFileDescriptor_, 0) == 0;
}

ConnectionPeer ConnectionPeer::acceptClient()
{
    int socketToClientFileDescriptor = accept(socketFileDescriptor_,
            nullptr, nullptr);

    ConnectionPeer peer;
    peer.socketFileDescriptor_ = socketToClientFileDescriptor;

    return peer;
}

bool ConnectionPeer::write(std::string message)
{
    return ::write(socketFileDescriptor_, message.data(), message.size()) != -1;
}

bool ConnectionPeer::active() const
{
    return socketFileDescriptor_ != -1;
}

std::string ConnectionPeer::peek()
{
    const int BUFFER_SIZE = 4096;
    char buffer[BUFFER_SIZE];
    int read = 0;

//    do {
        read = recv(socketFileDescriptor_, buffer, BUFFER_SIZE - 1, 0);

        if (read >= 0) {
            buffer[read] = '\0';
            buffer_ += buffer;
        }
//    } while (read > 0);

    return buffer_;
}

std::string ConnectionPeer::read(size_t length)
{
    while (buffer_.size() < length) {
        peek();
    }

    std::string message = buffer_.substr(0, length);
    buffer_ = buffer_.substr(length);
    return message;
}

void ConnectionPeer::close()
{
    ::close(socketFileDescriptor_);
    socketFileDescriptor_ = -1;
}
