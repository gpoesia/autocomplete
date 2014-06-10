#include <iostream>
#include <fstream>
#include <limits>
#include "autocompleteserver.h"
#include "stringlist.h"

using namespace std;

int main(int argc, char *argv[]) {
    if (argc != 4) {
        cerr << "Usage: " << argv[0] << " <<port>> <<max_clients>> <<words_file>>\n";
        return EXIT_FAILURE;
    }

    int port = atoi(argv[1]);
    int maxClients = atoi(argv[2]);

    ifstream wordsFile(argv[3]);

    if (!wordsFile) {
        cerr << "Coult not open words file '" << argv[2] << "'.\n";
        return EXIT_FAILURE;
    }

    string line;
    StringList stringList;

    while (getline(wordsFile, line)) {
        stringList.addString(line);
    }

    AutocompleteServer server(stringList);
    server.run(port, maxClients);

    return 0;
}
