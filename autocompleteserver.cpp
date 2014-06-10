#include <chrono>
#include <functional>
#include <thread>
#include <iostream>

#include "autocompleteserver.h"
#include "autocompleteprotocol.h"
#include "connectionpeer.h"

AutocompleteServer::AutocompleteServer(const StringList &dictionary) :
    dictionary_(dictionary)
{
}

void HandleClient(ConnectionPeer peer, StringList &dictionary)
{
    size_t messageLength = 0;

    bool closed = false;

    do {
        while ((messageLength = MessageLength(peer.peek())) == 0) {
            std::this_thread::yield();
        }

        std::chrono::system_clock::time_point before = std::chrono::system_clock::now();

        std::string message = peer.read(messageLength);
        std::string query = DecodeQuery(message);

        closed = query.size() == 0;

        if (!closed) {
            auto response = dictionary.possibleCompletions(query);
            peer.write(FormatResponse(response));
        }

        std::cout << "S_A "
                  << std::chrono::duration_cast<std::chrono::microseconds>(
                        std::chrono::system_clock::now() - before).count()
                  << '\n';
    } while (!closed);

    peer.close();
}

void AutocompleteServer::run(int port, int maxClients)
{
    ConnectionPeer serverPeer;
    serverPeer.listen(port);

    while (maxClients--) {
        ConnectionPeer client = serverPeer.acceptClient();
        auto clientThread = std::thread(HandleClient, client, std::ref(dictionary_));
        clientThread.detach();
    }
}
