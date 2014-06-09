#include <gtest/gtest.h>
#include "../connectionpeer.h"

#include <string>
#include <thread>
#include <functional>
#include <vector>
#include <array>
#include <iostream>

using namespace std;

const int ECHO_SERVER_PORT = 12344;
const int ECHO_SERVER_TEST_ITERATIONS = 100;
const int NUMBER_OF_CLIENTS = 10;

string RandomString(int length)
{
    string s;

    while (length--)
        s += static_cast<char>(rand() % 26 + 'A');

    return s;
}

void EchoClient(bool *success)
{
    ConnectionPeer client;
    *success = false;

    if (client.connect("127.0.0.1", ECHO_SERVER_PORT)) {
        int i = 0;

        while (i < ECHO_SERVER_TEST_ITERATIONS) {
            string message = RandomString(i + 1) + '\n';
            client.write(message);
            string response = client.read(message.size());

            // Error
            if (response != message)
                break;

            i++;
        }

        // Send exit message
        client.write("/quit");
        client.close();

        *success = (i == ECHO_SERVER_TEST_ITERATIONS);
    }
}

void EchoServerClientHandler(ConnectionPeer peer)
{
    while (true) {
        std::string message = peer.peek();

        size_t firstLineFeed;

        if (message == "/quit") {
            peer.close();
            break;
        } else if ((firstLineFeed = message.find_first_of('\n')) !=
                string::npos) {
            std::string message = peer.read(firstLineFeed + 1);
            peer.write(message);
        }
    }
}

void EchoServer(int maxClients)
{
    ConnectionPeer server;
    if (!server.listen(ECHO_SERVER_PORT))
        cerr << "Server failed listening...\n";

    vector<thread> handlerThreads;

    while (maxClients--) {
        ConnectionPeer peer = server.acceptClient();
        handlerThreads.push_back(thread(EchoServerClientHandler, peer));
    }

    for (int i = 0; i < handlerThreads.size(); i++)
        handlerThreads[i].join();
}

TEST(ConnectionTest, EchoTest)
{
    std::thread serverThread(EchoServer, NUMBER_OF_CLIENTS);

    vector<thread> clientThreads(NUMBER_OF_CLIENTS);
    array<bool, NUMBER_OF_CLIENTS> clientSuccess;

    for (int i = 0; i < clientThreads.size(); i++) {
        clientThreads[i] = thread(EchoClient, &clientSuccess[i]);
    }

    serverThread.join();

    int successfulClients = 0;

    for (int i = 0; i < clientThreads.size(); i++) {
        clientThreads[i].join();
        successfulClients += clientSuccess[i];
    }

    EXPECT_EQ(NUMBER_OF_CLIENTS, successfulClients);
}
