#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "autocompleteclient.h"

using namespace std;
using namespace std::chrono;

std::string POSSIBLE_INITIAL_CHARACTERS = "abcdefghijklmnopqrstuvwxyz"
                                          "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

const int THINK_TIME = 500; // Wait for 1000 milliseconds before typing the next character

int main(int argc, char *argv[]) {
    srand(time(nullptr));

    std::string serverAddress;
    int serverPort;

    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <<host>> <<port>>\n"
             << "Output is the mean response time for all queries.\n";
        return EXIT_FAILURE;
    }

    serverAddress = argv[1];
    serverPort = atoi(argv[2]);

    AutocompleteClient client;

    if (!client.connect(serverAddress, serverPort)) {
        cerr << "Error while connecting to server.\n";

        return EXIT_FAILURE;
    } else {
        char firstChar = POSSIBLE_INITIAL_CHARACTERS[
            rand() % POSSIBLE_INITIAL_CHARACTERS.size()];

        unsigned int responseTime = 0;

        system_clock::time_point before;

        before = system_clock::now();
        vector<string> possibleWords = client.complete(string() + firstChar);
        responseTime += duration_cast<milliseconds>(system_clock::now() - before).count();

        // Choose one random word to simulate typing
        string typedWord = possibleWords[rand() % possibleWords.size()];

        for (int i = 1; i < typedWord.size(); i++) {
            before = system_clock::now();

            if (client.complete(typedWord.substr(0, i)).size() == 0) {
                cerr << "Error getting response from server.\n";
            }

            responseTime += duration_cast<milliseconds>(system_clock::now() - before).count();
        }

        // Close connection
        client.disconnect();

        cout.setf(ios::fixed);
        cout << (static_cast<double>(responseTime) / typedWord.size()) << '\n';
    }

    return 0;
}
