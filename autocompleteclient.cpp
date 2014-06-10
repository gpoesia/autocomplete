#include "autocompleteclient.h"
#include "autocompleteprotocol.h"
#include <thread>
#include <chrono>

AutocompleteClient::AutocompleteClient() :
    peer_()
{
}

bool AutocompleteClient::connect(std::string server, int port)
{
    return peer_.connect(server, port);
}

void AutocompleteClient::disconnect()
{
    peer_.write(FormatCloseConnection());
    peer_.close();
}

std::vector<std::string> AutocompleteClient::complete(std::string prefix)
{
    peer_.write(FormatQuery(prefix));

    size_t messageLength = 0;

    while ((messageLength = MessageLength(peer_.peek())) == 0) {
        std::this_thread::yield();
    }

    std::string response = peer_.read(messageLength);
    return DecodeResponse(response);
}
