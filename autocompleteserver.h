#ifndef AUTOCOMPLETESERVER_H
#define AUTOCOMPLETESERVER_H

#include "stringlist.h"

class AutocompleteServer
{
    public:
        AutocompleteServer(const StringList &dictionary);
        void run(int port, int maxClients);

    private:
        StringList dictionary_;
};

#endif
