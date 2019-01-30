#ifndef CLIENT_H
#define CLIENT_H

#include "iclient.h"

class Client : public IClient {
public:
    explicit Client(const std::string &IdNumber);

    ~Client() override;

    std::string getIdNumber() override;

    std::string getFullName() override;

    int getPriority() override;

    const std::string &getBiometricData() override;

    void setFullName(const std::string &fullName) override;

    void updatePriority(int priority) override;

    void updateBiometricData(const std::string &biometricData) override;

    bool verifyBiometricData(const std::string &biometricData, double threshold) override;

    void newPackage(const std::string &packageId) override;

    std::vector<std::string> awaitingPackages() override;

    void packagesCollected() override;

private:
    std::vector<std::string> packages;
    std::string m_IdNumber;
    std::string m_fullName;
    std::string m_biometricData;
    int m_priority;
};

#endif // CLIENT_H