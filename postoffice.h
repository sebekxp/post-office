#ifndef POSTOFFICE_H
#define POSTOFFICE_H
#include "ipostoffice.h"
#include <string>
#include <fstream>
#include <iostream>
#include "client.h"
class PostOffice : public IPostOffice {
public:

    explicit PostOffice(unsigned gate_count);

    ~PostOffice() override;

    std::vector<std::string> getStatus() override;

    std::shared_ptr<IClient> getClient(const std::string &identificationNumber) override;

    void enqueueClient(const std::shared_ptr<IClient> &client) override;

    void gateReady(unsigned gateIndex) override;

    void collectPackages(unsigned gateIndex) override;
private:
    unsigned gateCount;
    std::vector<std::shared_ptr<IClient>> queue;
    std::vector<std::shared_ptr<IClient>> gate;
    std::vector<std::shared_ptr<IClient>> clients;
};

#endif // POSTOFFICE_H