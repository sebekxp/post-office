//#include <ole.h>
#include "client.h"
#include "exceptions.h"

std::string Client::getIdNumber() {
    return m_IdNumber;
}

std::string Client::getFullName() {
    return m_fullName;
}

void Client::setFullName(const std::string &fullName) {
    this->m_fullName = fullName;
}

int Client::getPriority() {
    return m_priority;
}

void Client::updatePriority(int priority) {
    this->m_priority = priority;
}


const std::string &Client::getBiometricData() {
    return m_biometricData;
}

void Client::updateBiometricData(const std::string &biometricData) {
    for (char i : biometricData) {
        if (i == 'T' || i == 'C' || i == 'G' || i == 'A')
            continue;
        else
            throw IncorrectBiometricDataException("Passed sequence contains illegal DNA sequence characters");
    }

    this->m_biometricData = biometricData;
}

double similarityScore(char a, char b) {
    return (a == b) ? 3 : -3;
}

double findMax(const double array[], int length) {
    double max = array[0];

    for (int i = 1; i < length; i++) {
        if (array[i] > max) {

            max = array[i];
        }
    }
    return max;
}

bool Client::verifyBiometricData(const std::string &biometricData, double threshold) {
    for (char i : biometricData) {
        if (i == 'T' || i == 'C' || i == 'G' || i == 'A')
            continue;
        else
            throw IncorrectBiometricDataException("Passed sequence contains illegal DNA sequence characters");
    }
    int lengthSeqA = this->m_biometricData.length();
    int lengthSeqB = biometricData.length();

    double gapPenalty = 2;
    double matrix[lengthSeqA + 1][lengthSeqB + 1];

    for (int i = 0; i <= lengthSeqA; i++)
        for (int j = 0; j <= lengthSeqB; j++)
            matrix[i][j] = 0;

    double traceback[4] = {};

    //start populating matrix
    for (int i = 1; i <= lengthSeqA; i++) {
        for (int j = 1; j <= lengthSeqB; j++) {
            traceback[0] = matrix[i - 1][j - 1] + similarityScore(this->m_biometricData[i - 1], biometricData[j - 1]);
            traceback[1] = matrix[i - 1][j] - gapPenalty;
            traceback[2] = matrix[i][j - 1] - gapPenalty;
            traceback[3] = 0;
            matrix[i][j] = findMax(traceback, 4);
        }
    }
    // find the max score in the matrix
    double matrix_max = 0;
    int i_max = 0, j_max = 0;
    for (int i = 1; i <= lengthSeqA; i++) {
        for (int j = 1; j <= lengthSeqB; j++) {
            if (matrix[i][j] > matrix_max) {
                matrix_max = matrix[i][j];
                i_max = i;
                j_max = j;
            }
        }
    }

    double maxScore = matrix[i_max][j_max];
    double shorterSequence = (lengthSeqA < lengthSeqB) ? lengthSeqA : lengthSeqB;
    double normalizedScore = maxScore / shorterSequence;

    return normalizedScore > threshold;
}

void Client::newPackage(const std::string &packageId) {

    bool flagPackage = true;
    for (auto &it : packages)
        if (it == packageId)
            flagPackage = false;

    if (flagPackage)
        packages.push_back(packageId);
    else
        throw PackageExistsException("Package with specified ID already exists.");

}

std::vector<std::string> Client::awaitingPackages() {
    return this->packages;
}

void Client::packagesCollected() {
    this->packages.clear();
}

Client::Client(const std::string &IdNumber) {
    m_IdNumber = IdNumber;
    m_priority = 0;
    m_fullName = "";
    m_biometricData = "";
}

Client::~Client() {
    packages.clear();
}
