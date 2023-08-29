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

class OpenFileError : public std::runtime_error {
public:
    OpenFileError(const String& path) : std::runtime_error(get_msg(path)) {}

private:
    const char* get_msg(const String& path) {
        return ("Failed to open file in path: " + path).c_str();
    }
};

class WriteError : public std::runtime_error {
public:
    WriteError(const String& path, const uint8_t& written, const uint8_t& total) : std::runtime_error(get_msg(path, written, total)) {}

private:
    const char* get_msg(const String& path, const uint8_t& written, const uint8_t& total) {
        return ("Failed to write " + String(total) + " bytes to " + path + " (written " + String(written) + ").").c_str();
    }
};

class NotImplemented : public std::exception {
public:
    NotImplemented() : std::exception() {}
    const char* what() const noexcept override {
        return "Feature not implemented...";
    }
};

#endif