#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>

/**
 * Indicates a package with the same ID already exists, which should never happen.
 */
class PackageExistsException : std::exception {
public:
    explicit PackageExistsException(const std::string &msg);

    const char *what() const noexcept override;

private:
    std::string message;
};

/**
 * Thrown if characters other than 'C', 'T', 'G' or 'A' were encountered in the sequence.
 */
class IncorrectBiometricDataException : std::exception {
public:
    explicit IncorrectBiometricDataException(const std::string &msg);

    const char *what() const noexcept override;

private:
    std::string message;
};

/**
 * Thrown if the passed index is incorrect in this context.
 */
class IncorrectGateException : std::exception {
public:
    explicit IncorrectGateException(const std::string &msg);

    const char *what() const noexcept override;

private:
    std::string message;
};

#endif // EXCEPTIONS_H