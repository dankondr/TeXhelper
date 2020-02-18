#ifndef TEXHELPER_EXCEPTION_H
#define TEXHELPER_EXCEPTION_H

#include <stdexcept>

class Exception : public std::exception {
private:
    const char *message;
public:
    explicit Exception(const char *message) : message(message) {}

    [[nodiscard]] char const *what() const noexcept override {
        return message;
    }
};

#endif //TEXHELPER_EXCEPTION_H
