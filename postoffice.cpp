#include <algorithm>
#include "postoffice.h"
#include "client.h"
#include "exceptions.h"

std::vector<std::string> PostOffice::getStatus() {

    std::vector<std::string> status;
    for (auto &it : gate) {
        if (it != nullptr)
            status.push_back(it->getFullName());
        else
            status.emplace_back("");
    }
    return status;
}

std::shared_ptr<IClient> PostOffice::getClient(const std::string &identificationNumber) {
    for (auto &i :clients) {
        if (i->getIdNumber() == identificationNumber)
            return i;
    }
    clients.push_back(std::make_shared<Client>(identificationNumber));
    //clients.push_back(std::shared_ptr<IClient>(new Client(identificationNumber)));
    return clients.back();
}

void PostOffice::enqueueClient(const std::shared_ptr<IClient> &client) {
    bool itWas = false;
    for (auto &it: queue)
        if (it->getIdNumber() == client->getIdNumber())
            itWas = true;

    if (!itWas)
        queue.push_back(client);

    std::sort(queue.begin(), queue.end(),
              [](const std::shared_ptr<IClient> &left,
                 const std::shared_ptr<IClient> &right) {
                  return left->getPriority() > right->getPriority();
              });
}

void PostOffice::gateReady(unsigned gateIndex) {
    if (gateIndex >= gateCount)
        throw IncorrectGateException("Passed index is incorrect in this context.");
    if (!queue.empty()) {
        gate[gateIndex] = queue.front();
        queue.erase(queue.begin());
    }
    else
        gate[gateIndex] = nullptr;
}

void PostOffice::collectPackages(unsigned gateIndex) {
    if (gateIndex >= gateCount)
        throw IncorrectGateException("Passed index is incorrect in this context.");
    if (gate[gateIndex] == nullptr)
        throw IncorrectGateException("No client is being served in this gate.");

    gate[gateIndex]->packagesCollected();
}

std::shared_ptr<IPostOffice> IPostOffice::create(unsigned gate_count) {
    return std::shared_ptr<IPostOffice>(new PostOffice(gate_count));
}

PostOffice::PostOffice(unsigned gate_count) : gateCount(gate_count) {
    gate.push_back(nullptr);
    for (int i = 1; i < gate_count; ++i) {
        gate.push_back(nullptr);
    }

    std::ifstream myfile("example.txt");
    if (myfile.is_open()) {
        std::vector<std::string> dataFromFile;
        std::string line;
        int countLine = 0;
        while (std::getline(myfile, line)) {
            dataFromFile.push_back(line);
            countLine++;
        }
        for (int i = 0; i < countLine;) {
            std::string idNum = dataFromFile[i];
            std::string fullName = dataFromFile[i + 1];
            int priorNumb = std::atoi(dataFromFile[i + 2].c_str());
            std::string bioData = dataFromFile[i + 3];
            std::vector<std::string> package;
            int j = i + 4, countPackage = 0;
            while (dataFromFile[j].length() != 0) {
                package.push_back(dataFromFile[j]);
                j++;
                countPackage++;
            }
            i += 4 + countPackage + 1;
            auto tmpClient = this->getClient(idNum);
            tmpClient->setFullName(fullName);
            tmpClient->updatePriority(priorNumb);
            tmpClient->updateBiometricData(bioData);
            while (!package.empty()) {
                tmpClient->newPackage(package.front());
                package.erase(package.begin());
            }
        }
        myfile.close();
    }
}

PostOffice::~PostOffice() {
    std::ofstream myfile("example.txt");
    if (myfile.is_open()) {
        while (!clients.empty()) {
            myfile << clients.front()->getIdNumber() << '\n';
            myfile << clients.front()->getFullName() << '\n';
            myfile << clients.front()->getPriority() << '\n';
            myfile << clients.front()->getBiometricData() << '\n';
            auto vecPackage = clients.front()->awaitingPackages();
            while (!vecPackage.empty()) {
                myfile << vecPackage.front() << '\n';
                vecPackage.erase(vecPackage.begin());
            }
            myfile << '\n';
            clients.erase(clients.begin());
        }
        myfile.close();
    }
    gate.clear();
    clients.clear();
    queue.clear();
}

