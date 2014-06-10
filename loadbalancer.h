#ifndef LOADBALANCER_H
#define LOADBALANCER_H

#include <mutex>
#include <string>
#include <utility>
#include <vector>

#include "connectionpeer.h"

class LoadBalancer {
    public:
        LoadBalancer(const std::vector<std::pair<std::string, int>> &servers);

        void run(int port, int maxClients);

    private:
        const std::vector<std::pair<std::string, int>> serverAddresses_;
        unsigned int nextServer_;
};

#endif
