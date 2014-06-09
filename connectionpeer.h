#ifndef CONNECTIONPEER_H
#define CONNECTIONPEER_H

#include <string>
#include <functional>

/// A peer of a TCP connection.
class ConnectionPeer {
    public:
        ConnectionPeer();

        bool connect(std::string serverHostName, int port);

        bool listen(int port);
        ConnectionPeer acceptClient();

        std::string peek();
        std::string read(size_t length);
        bool write(std::string message);

        bool active() const;

        void close();

    private:
        int socketFileDescriptor_;
        std::string buffer_;
};

#endif
