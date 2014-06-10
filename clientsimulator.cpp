#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <cstdlib>
#include <thread>
#include <ctime>
#include "autocompleteclient.h"

using namespace std;
using namespace std::chrono;

std::string POSSIBLE_INITIAL_CHARACTERS = "abcdefghijklmnopqrstuvwxyz"
                                          "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

const int THINK_TIME = 200; // Wait for 200 milliseconds before typing the next character

int main(int argc, char *argv[]) {
    srand(clock());

    std::string serverAddress;
    int serverPort, words;

    if (argc != 4) {
        cerr << "Usage: " << argv[0] << " <<host>> <<port>> <<number_of_words>>\n";
        return EXIT_FAILURE;
    }

    serverAddress = argv[1];
    serverPort = atoi(argv[2]);
    words = atoi(argv[3]);

    AutocompleteClient client;

    if (!client.connect(serverAddress, serverPort)) {
        cerr << "Error while connecting to server.\n";

        return EXIT_FAILURE;
    } else {
        while (words--) {
            char firstChar = POSSIBLE_INITIAL_CHARACTERS[
                rand() % POSSIBLE_INITIAL_CHARACTERS.size()];

            system_clock::time_point before;

            before = system_clock::now();
            vector<string> possibleWords = client.complete(string() + firstChar);

            cout << "R "
                << duration_cast<microseconds>(system_clock::now() - before).count()
                << '\n';

            std::this_thread::sleep_for(std::chrono::milliseconds(THINK_TIME));

            // Choose one random word to simulate typing
            string typedWord = possibleWords[rand() % possibleWords.size()];

            cout << "L " << typedWord.size() << '\n';

            for (int i = 1; i < typedWord.size(); i++) {
                before = system_clock::now();

                if (client.complete(typedWord.substr(0, i)).size() == 0) {
                    cerr << "Error getting response from server.\n";
                }

                cout << "R "
                    << duration_cast<microseconds>(system_clock::now() - before).count()
                    << '\n';

                std::this_thread::sleep_for(std::chrono::milliseconds(THINK_TIME));
            }
        }

        // Close connection
        client.disconnect();
    }

    return 0;
}
