#ifndef IOT_EXCEPTIONS_H
#define IOT_EXCEPTIONS_H

#include <stdexcept>

class SerialUnintializedException : public std::runtime_error {
public:
    SerialUnintializedException() : std::runtime_error(msg) {}

private:
    const char* msg = "Serial is unintialized.";
};

class ComponentUninitializedException : public std::exception {
public:
    ComponentUninitializedException(const char* component) : msg(get_msg(component)) {}

    const char* what() const noexcept override {
        return msg;
    }

private:
    const char* msg;
    
    const char* get_msg(const char* component) {
        std::string msg = "Component ";
        msg += component;
        msg += " is uninitialized";
        return msg.c_str();
    }
};

#endif