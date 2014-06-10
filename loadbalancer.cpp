#include <thread>
#include <vector>
#include <chrono>

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

        std::string message = clientPeer.read(messageLength);
        serverPeer.write(message);

        close = (DecodeQuery(message).size() == 0);

        if (!close) {
            while ((messageLength = MessageLength(serverPeer.peek())) == 0)
                std::this_thread::sleep_for(std::chrono::milliseconds(50));

            message = serverPeer.read(messageLength);
            clientPeer.write(message);
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
