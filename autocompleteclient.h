#ifndef AUTOCOMPLETECLIENT_H
#define AUTOCOMPLETECLIENT_H

#include <string>
#include <vector>
#include "connectionpeer.h"

class AutocompleteClient
{
    public:
        AutocompleteClient();

        bool connect(std::string server, int port);
        void disconnect();

        std::vector<std::string> complete(std::string prefix);

    private:
        ConnectionPeer peer_;
};

#endif
