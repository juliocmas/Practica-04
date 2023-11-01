#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <algorithm>

class Router {
public:
    Router(const std::string& name) : name(name) {}

    void addRoute(const std::string& destination, int cost) {
        routingTable[destination] = cost;
    }

    void removeRoute(const std::string& destination) {
        routingTable.erase(destination);
    }

    void printRoutingTable() const {
        std::cout << "tabla de enrutamiento para" << name << ":\n";
        for (const auto& entry : routingTable) {
            std::cout << "Destination: " << entry.first << ", Cost: " << entry.second << "\n";
        }
    }

    void inputRoutingTable() {
        std::cout << "Ingrese la tabla de enrutamiento para " << name << ":\n";
        std::string destination;
        int cost;
        while (true) {
            std::cout << "Introduce el destino (o 'Enter' para terminar): ";
            std::cin >> destination;
            if (destination == "done") {
                break;
            }
            std::cout << "Enter cost: ";
            std::cin >> cost;
            routingTable[destination] = cost;
        }
    }

    const std::map<std::string, int>& getRoutingTable() const {
        return routingTable;
    }

private:
    std::string name;
    std::map<std::string, int> routingTable;
};

class Network {
public:
    void addRouter(const std::string& name) {
        routers.emplace_back(name);
    }

    void removeRouter(const std::string& name) {
        routers.erase(std::remove_if(routers.begin(), routers.end(), [&](const Router& router) {
            return routerName(router) == name;
        }), routers.end());
    }

    void loadTopologyFromFile(const std::string& filename) {
        std::ifstream file(filename);
        std::string source, destination;
        int cost;
        while (file >> source >> destination >> cost) {
            networkTopology[source + " -> " + destination] = cost;
            networkTopology[destination + " -> " + source] = cost;
        }
    }

    void updateRoutingTables() {
        for (auto& router : routers) {
            router.updateRoutingTable(networkTopology);
        }
    }

    void printNetworkTopology() const {
        std::cout << "Network Topology:\n";
        for (const auto& entry : networkTopology) {
            std::cout << entry.first << ", Cost: " << entry.second << "\n";
        }
    }

    void printRoutingTables() const {
        for (const auto& router : routers) {
            router.printRoutingTable();
        }
    }

    Router& getRouter(int index) {
        return routers.at(index);
    }

    std::map<std::string, int> getCostTableFromRouter(const std::string& routerName) const {
        std::map<std::string, int> costTable;
        Router router = findRouter(routerName);
        if (!router.getRoutingTable().empty()) {
            for (const auto& entry : router.getRoutingTable()) {
                costTable[entry.first] = entry.second;
            }
        }
        return costTable;
    }

    Router findRouter(const std::string& name) const {
        auto it = std::find_if(routers.begin(), routers.end(), [&](const Router& router) {
            return routerName(router) == name;
        });

        if (it != routers.end()) {
            return *it;
        } else {
            return Router("InvalidRouter");
        }
    }

private:
    std::vector<Router> routers;
    std::map<std::string, int> networkTopology;
};

int main() {
    Network network;

    int numRouters;
    std::cout << "Ingrese la cantidad de enrutadores: ";
    std::cin >> numRouters;

    for (int i = 1; i <= numRouters; ++i) {
        network.addRouter("Router" + std::to_string(i));
    }

    std::string topologyFilename;
    std::cout << "Ingrese la ubicación del archivo de topología de red:";
    std::cin >> topologyFilename;
    network.loadTopologyFromFile(topologyFilename);

    for (int i = 0; i < numRouters; ++i) {
        Router& router = network.getRouter(i);
        router.inputRoutingTable();
    }

    network.updateRoutingTables();

    network.printNetworkTopology();
    network.printRoutingTables();

    std::string selectedRouter;
    std::cout << "Ingrese el nombre del enrutador para ver la tabla de costos: ";
    std::cin >> selectedRouter;

    std::map<std::string, int> costTable = network.getCostTableFromRouter(selectedRouter);
    std::cout << "tabla de costos de " << selectedRouter << ":\n";
    for (const auto& entry : costTable) {
        std::cout << "Destino: " << entry.first << ", Cost: " << entry.second << "\n";
    }

    return 0;
}
