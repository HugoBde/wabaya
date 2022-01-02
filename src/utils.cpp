#include "utils.h"

#include <iostream>

void log_error(std::string message) {
    std::cout << "\x1b[31;1m[ERROR]" << message << "\x1b[m\n";
}

void log_success(std::string message) {
    std::cout << "\x1b[32;1m" << message << "\x1b[m\n";
}

void log_warning(std::string message) {
    std::cout << "\x1b[33;1m[WARNING]" << message << "\x1b[m\n";
}