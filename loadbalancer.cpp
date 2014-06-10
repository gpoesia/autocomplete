#include <thread>
#include <vector>
#include <chrono>
#include <iostream>

#include "loadbalancer.h"
#include "autocompleteprotocol.h"

LoadBalancer::LoadBalancer(const std::vector<std::pair<std::string, int>> &servers) :
    serverAddresses_(servers),
    nextServer_(0)
{
}

void HandleClient(ConnectionPeer clientPeer, ConnectionPeer serverPeer)
{
    bool close = false;
    size_t messageLength = 0;

    while (!close) {
        while ((messageLength = MessageLength(clientPeer.peek())) == 0)
            std::this_thread::yield();

        std::chrono::system_clock::time_point before = std::chrono::system_clock::now();

        std::string message = clientPeer.read(messageLength);
        serverPeer.write(message);

        close = (DecodeQuery(message).size() == 0);

        std::cout << "S_LB "
                  << std::chrono::duration_cast<std::chrono::microseconds>(
                          std::chrono::system_clock::now() - before).count()
                  << '\n';

        if (!close) {
            while ((messageLength = MessageLength(serverPeer.peek())) == 0)
                std::this_thread::yield();

            before = std::chrono::system_clock::now();

            message = serverPeer.read(messageLength);
            clientPeer.write(message);

            std::cout << "S_LB "
                      << std::chrono::duration_cast<std::chrono::microseconds>(
                              std::chrono::system_clock::now() - before).count()
                      << '\n';
        }
    }

    clientPeer.close();
    serverPeer.close();
}

void LoadBalancer::run(int port, int maxClients)
{
    ConnectionPeer loadBalancingServer;

    loadBalancingServer.listen(port);

    while (maxClients--) {
        ConnectionPeer nextClient = loadBalancingServer.acceptClient();

        ConnectionPeer nextServerConnection;
        nextServerConnection.connect(serverAddresses_[nextServer_].first,
                serverAddresses_[nextServer_].second);

        std::thread clientHandler(HandleClient, nextClient, nextServerConnection);
        clientHandler.detach();

        nextServer_ = (nextServer_ + 1) % serverAddresses_.size();
    }

    loadBalancingServer.close();
}
