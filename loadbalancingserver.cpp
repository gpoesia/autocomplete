#include <iostream>
#include <fstream>
#include <limits>
#include <limits>
#include "loadbalancer.h"

using namespace std;

int main(int argc, char *argv[]) {
    if (argc < 4) {
        cerr << "Usage: " << argv[0] << " <<port>> <<address_server_1>> <<port_server_1>> [<<address_server_2>> <<port_server_2>> ...]\n";
        return EXIT_FAILURE;
    }

    int port = atoi(argv[1]);
    vector<pair<string, int>> serverAddresses;

    for (int i = 2; i + 1 < argc; i += 2) {
        serverAddresses.push_back(make_pair(argv[i], atoi(argv[i+1])));
    }

    LoadBalancer loadBalancer(serverAddresses);

    loadBalancer.run(port, numeric_limits<int>::max());

    return 0;
}
